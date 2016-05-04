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

namespace argcv {
namespace wrapper {
namespace leveldb {

typedef bool (*kv_handler)(const std::string&, const std::string&, void*);

typedef struct ldb_info {
} ldb_info;

bool ldb_wr::conn() { return false; }

bool ldb_wr::close() { return true; }

bool ldb_wr::destroy(const std::string& ddir) { return true; }

ldb_wr::~ldb_wr() {}

bool ldb_wr::start_with(const std::string& base, kv_handler kvs, void* data) { return false; }
bool ldb_wr::exist(const std::string& key) { return false; }
bool ldb_wr::put(const std::string& key, const std::string& val) { return false; }
bool ldb_wr::get(const std::string& key, std::string* _val) { return false; }
bool ldb_wr::rm(const std::string& key) { return false; }

bool ldb_wr::batch_put(const std::map<std::string, std::string>& kvs) { return false; }
bool ldb_wr::batch_rm(const std::set<std::string>& keys) { return false; }
}
}
}