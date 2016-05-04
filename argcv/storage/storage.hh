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
#ifndef ARGCV_STORAGE_STORAGE_HH
#define ARGCV_STORAGE_STORAGE_HH
#include <map>
#include <set>
#include <string>
#include <vector>

namespace argcv {
namespace storage {

class storage {
public:
    class stg_bw_handler {
    public:
        virtual ~stg_bw_handler() {}
        virtual void put(const std::string& key, const std::string& val = "",
                         const std::string& selector = "")
            = 0;
        virtual void rm(const std::string& key, const std::string& selector = "") = 0;
        virtual bool commit() = 0;
        virtual bool _err() = 0;
    };

public:
    virtual bool conn() = 0;
    virtual bool close() = 0;
    virtual bool is_closed() = 0;

    virtual bool start_with(const std::string& base,
                            bool (*kv_handler)(const std::string&, const std::string&, void*),
                            void* data = nullptr, const std::string& selector = "")
        = 0;

    virtual bool exist(const std::string& key, const std::string& selector = "") = 0;
    virtual bool put(const std::string& key, const std::string& val = "", const std::string& selector = "")
        = 0;
    virtual bool get(const std::string& key, std::string* _val, const std::string& selector = "") = 0;
    virtual bool rm(const std::string& key, const std::string& selector = "") = 0;

    virtual stg_bw_handler* batch_writer() = 0;

    virtual bool batch_put(const std::map<std::string, std::pair<std::string, std::string>>& kvs) = 0;
    virtual bool batch_rm(const std::set<std::pair<std::string, std::string>>& keys) = 0;
};

typedef storage::stg_bw_handler stg_bw_handler;
}
}  //  namespace argcv::storage

#endif  //  ARGCV_STORAGE_STORAGE_HH