/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2015 Yu Jing <yu@argcv.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 **/
#include "argcv/wrapper/leveldb_wr.hh"

#include <map>
#include <set>

#include "leveldb/db.h"
#include "leveldb/cache.h"
#include "leveldb/options.h"
#include "leveldb/write_batch.h"

namespace argcv {
namespace wrapper {
namespace leveldb {

typedef bool (*kv_handler)(const std::string&, const std::string&, void*);

// low level apis
::leveldb::DB* ldb_init(const std::string& ddir, ::leveldb::Options* _opt, size_t cache_size = 0,
                        bool create_if_missing = true);
bool ldb_close(::leveldb::DB* db, ::leveldb::Options* _opt);

bool ldb_start_with(::leveldb::DB* db, const std::string& base, kv_handler kvs, void* data = nullptr);
bool ldb_exist(::leveldb::DB* db, const std::string& key);
bool ldb_put(::leveldb::DB* db, const std::string& key, const std::string& val = "");
bool ldb_get(::leveldb::DB* db, const std::string& key, std::string* _val);
bool ldb_rm(::leveldb::DB* db, const std::string& key);

bool ldb_batch_put(::leveldb::DB* db, const std::map<std::string, std::string>& kvs);
bool ldb_batch_rm(::leveldb::DB* db, const std::set<std::string>& keys);

bool ldb_destroy(const std::string& ddir);

/////////////////////////////////////////////////////////////////////////////////////

typedef struct ldb_info {
    ::leveldb::DB* _db;
    ::leveldb::Options opt;
} ldb_info;

typedef struct ldb_info_w { ::leveldb::WriteBatch* _h; } ldb_info_w;
/////////////////////////////////////////////////////////////////////////////////////

ldb_wr::bw_handler::bw_handler(ldb_info* _info) : _info(_info) {
    if (_info != nullptr && _info->_db != nullptr) {
        _w = new ldb_info_w;
        _w->_h = new ::leveldb::WriteBatch;
    } else {
        _w = nullptr;
    }
}

ldb_wr::bw_handler::~bw_handler() {
    if (_w != nullptr) {
        delete _w->_h;
        delete _w;
    }
}

void ldb_wr::bw_handler::put(const std::string& key, const std::string& val) { _w->_h->Put(key, val); }
void ldb_wr::bw_handler::rm(const std::string& key) {
    _w->_h->Delete(key);
    // return true;
}
bool ldb_wr::bw_handler::commit() {
    bool rtv = ((_info->_db->Write(::leveldb::WriteOptions(), _w->_h)).ok());
    _w->_h->Clear();
    return rtv;
}

/////////////////////////////////////////////////////////////////////////////////////
bool ldb_wr::conn() {
    if (!is_closed()) {
        return false;
    } else {
        _info = new ldb_info;
        _info->_db = ldb_init(ddir, &(_info->opt), cache_size, create_if_missing);
        return _info->_db != nullptr;
    }
}

bool ldb_wr::close() {
    // printf("close start ..\n"); fflush(NULL);
    if (is_closed()) {
        // printf("also closed ..\n"); fflush(NULL);
        return false;
    } else {
        // printf("try do close start1 ..\n"); fflush(NULL);
        ldb_close(_info->_db, &(_info->opt));
        // printf("try do close start2 ..\n"); fflush(NULL);
        delete _info;
        // printf("try do close start3 ..\n"); fflush(NULL);
        _info = nullptr;
        // printf("try do close finished..\n"); fflush(NULL);
        return true;
    }
}

bool ldb_wr::destroy(const std::string& ddir) { return ldb_destroy(ddir); }

ldb_wr::~ldb_wr() {
    if (!is_closed()) {
        this->close();
    }
}

bool ldb_wr::start_with(const std::string& base, kv_handler kvs, void* data) {
    return ldb_start_with(_info->_db, base, kvs, data);
}
bool ldb_wr::exist(const std::string& key) { return ldb_exist(_info->_db, key); }
bool ldb_wr::put(const std::string& key, const std::string& val) { return ldb_put(_info->_db, key, val); }
bool ldb_wr::get(const std::string& key, std::string* _val) { return ldb_get(_info->_db, key, _val); }
bool ldb_wr::rm(const std::string& key) { return ldb_rm(_info->_db, key); }

bool ldb_wr::batch_put(const std::map<std::string, std::string>& kvs) {
    return ldb_batch_put(_info->_db, kvs);
}
bool ldb_wr::batch_rm(const std::set<std::string>& keys) { return ldb_batch_rm(_info->_db, keys); }

/////////////////////////////////////////////////////////////////////////////////////
::leveldb::DB* ldb_init(const std::string& ddir, ::leveldb::Options* _opt, size_t cache_size,
                        bool create_if_missing) {
    if (_opt == nullptr) {
        return nullptr;
    }
    _opt->create_if_missing = create_if_missing;
    if (cache_size > 0) {
        _opt->block_cache = ::leveldb::NewLRUCache(cache_size);
    }
    ::leveldb::DB* db;
    return ::leveldb::DB::Open(*_opt, ddir, &db).ok() ? db : nullptr;
}

bool ldb_close(::leveldb::DB* db, ::leveldb::Options* _opt) {
    if (db != nullptr) {
        delete db;
    }
    if (_opt != nullptr && _opt->block_cache != nullptr) {
        delete _opt->block_cache;
    }
    return true;
}

bool ldb_start_with(::leveldb::DB* db, const std::string& base, kv_handler kvs, void* data) {
    ::leveldb::ReadOptions snap_read_opt;
    snap_read_opt.snapshot = db->GetSnapshot();
    ::leveldb::Iterator* it = db->NewIterator(snap_read_opt);
    ::leveldb::Slice slice = base;
    for (it->Seek(slice);
         it->Valid()
         && std::mismatch(base.begin(), base.end(), it->key().ToString().begin()).first == base.end();
         it->Next()) {
        if (!kvs(it->key().ToString(), it->value().ToString(), data)) {
            break;
        }
    }
    delete it;
    db->ReleaseSnapshot(snap_read_opt.snapshot);
    return true;
}

bool ldb_exist(::leveldb::DB* db, const std::string& key) {
    std::string val;
    return ((db->Get(::leveldb::ReadOptions(), key, &val)).ok());
}

bool ldb_put(::leveldb::DB* db, const std::string& key, const std::string& val) {
    return ((db->Put(::leveldb::WriteOptions(), key, val)).ok());
}

bool ldb_get(::leveldb::DB* db, const std::string& key, std::string* _val) {
    return ((db->Get(::leveldb::ReadOptions(), key, _val)).ok());
}

bool ldb_rm(::leveldb::DB* db, const std::string& key) {
    return ((db->Delete(::leveldb::WriteOptions(), key)).ok());
}

bool ldb_batch_put(::leveldb::DB* db, const std::map<std::string, std::string>& kvs) {
    ::leveldb::WriteBatch* _wb = new ::leveldb::WriteBatch;
    for (std::map<std::string, std::string>::const_iterator it = kvs.begin(); it != kvs.end(); it++) {
        // printf("ldb_batch_add %s -> %s\n", it->first.c_str(), it->second.c_str());
        _wb->Put(it->first, it->second);
    }
    bool rst = ((db->Write(::leveldb::WriteOptions(), _wb)).ok());
    delete _wb;
    return rst;
}

bool ldb_batch_rm(::leveldb::DB* db, const std::set<std::string>& keys) {
    ::leveldb::WriteBatch* _wb = new ::leveldb::WriteBatch;
    for (std::set<std::string>::const_iterator it = keys.begin(); it != keys.end(); it++) {
        _wb->Delete(*it);
    }
    bool rst = ((db->Write(::leveldb::WriteOptions(), _wb)).ok());
    delete _wb;
    return rst;
}

bool ldb_destroy(const std::string& ddir) { return ::leveldb::DestroyDB(ddir, ::leveldb::Options()).ok(); }
}
}
}