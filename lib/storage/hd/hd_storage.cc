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
#include "argcv/storage/hd_storage.hh"

#include <cstdio>
#include <cassert>

#include <map>
#include <set>
#include <string>

#include "argcv/storage/storage.hh"
#include "argcv/wrapper/leveldb_wr.hh"
#include "argcv/wrapper/ini_wr.hh"

namespace argcv {
namespace storage {
using ::argcv::wrapper::ini::ini_wr;
using ::argcv::wrapper::leveldb::ldb_wr;
using ::argcv::wrapper::leveldb::bw_handler;

hd_storage::hd_storage(const std::string& config) : config(config) {
    ini_wr cfg(config);
    err_no = 0;
    // static_cast<std::string>(cfg["storage"]["prefix"]);
    db_prefix = static_cast<std::string>(cfg["storage"]["prefix"]);
    sz_shard = atoi(static_cast<std::string>(cfg["storage"]["shard"]).c_str());
    if (sz_shard < 1) sz_shard = 1;
    sz_cache = atoi(static_cast<std::string>(cfg["storage"]["cache"]).c_str()) * 1024;  // MB

    std::string s(db_prefix);
    s += std::to_string(0);
    ldb_wr* _cdb = new ldb_wr(s, sz_cache);
    if (_cdb == nullptr) {
        err_no = -1;
    }
    _db = _cdb;
    assert(err_no == 0);
}

hd_storage::~hd_storage() {
    delete _db;
}

bool hd_storage::conn() { return _db->conn(); }
bool hd_storage::close() { return _db->close(); }
bool hd_storage::is_closed() { return _db->is_closed(); }

bool hd_storage::start_with(const std::string& base,
                            bool (*kv_handler)(const std::string&, const std::string&, void*), void* data,
                            const std::string& selector) {
    return _db->start_with(base, kv_handler, data);
}
bool hd_storage::exist(const std::string& key, const std::string& selector) { return _db->exist(key); }
bool hd_storage::put(const std::string& key, const std::string& val, const std::string& selector) {
    return _db->put(key, val);
}
bool hd_storage::get(const std::string& key, std::string* _val, const std::string& selector) {
    return _db->get(key, _val);
}
bool hd_storage::rm(const std::string& key, const std::string& selector) { return _db->rm(key); }

bool hd_storage::batch_put(const std::map<std::string, std::pair<std::string, std::string>>& kvs) {
    std::map<std::string, std::string> kvs_to_add;
    for (std::map<std::string, std::pair<std::string, std::string>>::const_iterator it = kvs.begin();
         it != kvs.end(); it++) {
        kvs_to_add[it->first] = it->second.second;
    }
    return _db->batch_put(kvs_to_add);
}
bool hd_storage::batch_rm(const std::set<std::pair<std::string, std::string>>& keys) {
    std::set<std::string> keys_to_rm;
    for (std::set<std::pair<std::string, std::string>>::const_iterator it = keys.begin(); it != keys.end();
         it++) {
        keys_to_rm.insert(it->first);
    }
    return _db->batch_rm(keys_to_rm);
}

stg_bw_handler* hd_storage::batch_writer() { return new hd_bw_handler(_db); }

const size_t hd_storage::_cache_size() const { return _db->_cache_size(); }
const bool hd_storage::_create_if_missing() const { return _db->_create_if_missing(); }
}
}  //  namespace argcv::storage
