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
#ifndef ARGCV_STORAGE_HD_STORAGE_HH
#define ARGCV_STORAGE_HD_STORAGE_HH

#include <map>
#include <set>
#include <string>
#include <vector>

#include "argcv/storage/storage.hh"

#include "argcv/wrapper/leveldb_wr.hh"

namespace argcv {
namespace storage {

class hd_storage : public storage {
public:
    class hd_bw_handler : public stg_bw_handler {
    public:
        hd_bw_handler(::argcv::wrapper::leveldb::ldb_wr* _db) : _bwh(_db->batch_writer()) {}
        ~hd_bw_handler() { delete _bwh; }

        void put(const std::string& key, const std::string& val = "", const std::string& selector = "") {
            _bwh->put(key, val);
        }
        void rm(const std::string& key, const std::string& selector = "") { _bwh->rm(key); }
        bool commit() { return _bwh->commit(); }
        bool _err() { return _bwh->_err(); }

    private:
        ::argcv::wrapper::leveldb::bw_handler* _bwh;
    };

public:
    hd_storage(const std::string& config);

    virtual ~hd_storage();

    bool conn();
    bool close();
    bool is_closed();

    bool start_with(const std::string& base,
                    bool (*kv_handler)(const std::string&, const std::string&, void*), void* data = nullptr,
                    const std::string& selector = "");
    bool exist(const std::string& key, const std::string& selector = "");
    bool put(const std::string& key, const std::string& val = "", const std::string& selector = "");
    bool get(const std::string& key, std::string* _val, const std::string& selector = "");
    bool rm(const std::string& key, const std::string& selector = "");

    bool batch_put(const std::map<std::string, std::pair<std::string, std::string>>& kvs);
    bool batch_rm(const std::set<std::pair<std::string, std::string>>& keys);

    stg_bw_handler* batch_writer();

    const size_t _cache_size() const;
    const bool _create_if_missing() const;

    const int _err_no() { return err_no; }

private:
    const std::string config;
    std::string db_prefix;
    size_t sz_shard;
    size_t sz_cache;
    int err_no;
    ::argcv::wrapper::leveldb::ldb_wr* _db;
};

typedef hd_storage::hd_bw_handler hd_bw_handler;
}
}  //  namespace argcv::storage

#endif  //  ARGCV_STORAGE_HD_STORAGE_HH