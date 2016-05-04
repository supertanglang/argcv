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
#ifndef ARGCV_WRAPPER_LEVELDB_WR_HH
#define ARGCV_WRAPPER_LEVELDB_WR_HH

#include <map>
#include <set>
#include <string>

//#include "leveldb/db.h"
//#include "leveldb/cache.h"
//#include "leveldb/options.h"

namespace argcv {
namespace wrapper {
namespace leveldb {

struct ldb_info;
struct ldb_info_w;

class ldb_wr {
public:
    class bw_handler {
    public:
        bw_handler(ldb_info* _info);
        virtual ~bw_handler();

        void put(const std::string& key, const std::string& val = "");
        void rm(const std::string& key);
        bool commit();
        bool _err() { return _w != nullptr; }

    private:
        ldb_info* _info;
        ldb_info_w* _w;
    };

public:
    ldb_wr(const std::string& ddir, size_t cache_size = 0, bool create_if_missing = true)
        : ddir(ddir), cache_size(cache_size), create_if_missing(create_if_missing) {
        _info = nullptr;
    }

    bool conn();
    bool close();
    bool is_closed() { return _info == nullptr; }

    // WARNING : WILL ERASE ALL DATA IN THIS DIR
    // SHALL CLOSE THE DB BEFORE CALL THIS FUNCTION
    static bool destroy(const std::string& ddir);

    virtual ~ldb_wr();

    bool start_with(const std::string& base,
                    bool (*kv_handler)(const std::string&, const std::string&, void*), void* data = nullptr);
    bool exist(const std::string& key);
    bool put(const std::string& key, const std::string& val = "");
    bool get(const std::string& key, std::string* _val);
    bool rm(const std::string& key);

    bool batch_put(const std::map<std::string, std::string>& kvs);
    bool batch_rm(const std::set<std::string>& keys);

    bw_handler * batch_writer() { return new bw_handler(_info); }

    const size_t _cache_size() const { return cache_size; }
    const bool _create_if_missing() const { return create_if_missing; }

private:
    const std::string ddir;
    size_t cache_size;
    bool create_if_missing;
    ldb_info* _info;

    class ldb_wr_agent {
    public:
        ldb_wr_agent(ldb_wr& m_o, const std::string& m_k) : o(m_o), k(m_k) {}

        // get value of key
        operator std::string() const {
            std::string v;
            o.get(k, &v);
            return v;
        }

        // exist this value
        operator bool() const { return o.exist(k); }

        void operator=(const std::string& v) { o.put(k, v); }

    private:
        ldb_wr& o;
        const std::string k;
    };

public:
    ldb_wr_agent operator[](const std::string& k) { return ldb_wr_agent(*this, k); }
};

typedef ldb_wr::bw_handler bw_handler;

/*

*/
}
}
}  // namespace argcv::wrapper::leveldb

#endif  // ARGCV_WRAPPER_LEVELDB_WR_HH
