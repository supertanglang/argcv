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
#ifndef ARGCV_TEST_WRAPPER_LEVELDB_WR_TEST_HH
#define ARGCV_TEST_WRAPPER_LEVELDB_WR_TEST_HH
#include "test/argcv_test.h"

#include <map>
#include <set>
#include <string>

#include "argcv/wrapper/leveldb_wr.hh"
using namespace argcv::string;
using namespace argcv::wrapper::leveldb;

bool test_case_leveldb_wr_key_value_printer(const std::string& k, const std::string& v, void* data) {
    int* _offset = (int*)data;
    printf("%d key: %s \t value: %s \n", (*_offset)++, k.c_str(), v.c_str());
    return true;  //(*_offset) < 2;
}

static int test_case_leveldb_wr(int argc, char* argv[]) {
    const char* ddir = "test_case_leveldb_wr.data";
    ldb_wr lw(ddir, 0, true);
    lw.conn();
    lw.put("a", "00");
    lw.put("a01", "01");
    lw.put("a02", "02");
    // lw.put("a03", "03");
    printf("destroy status : %d \n", ldb_wr::destroy(ddir));
    lw.put("a04", "04");
    lw.put("b01", "04");
    lw.put("b03", "05");
    printf("exist a? %d \n", lw.exist("a") ? 1 : 0);

    std::map<std::string, std::string> kvs_to_put;
    kvs_to_put["a07"] = "x07";
    kvs_to_put["a08"] = "x08";
    kvs_to_put["a09"] = "x09";
    kvs_to_put["a10"] = "";

    std::set<std::string> keys_to_del;
    keys_to_del.insert("a04");

    lw.batch_put(kvs_to_put);
    lw.batch_rm(keys_to_del);

    lw.rm("a02");
    int i = 0;

    lw["a80"] = "80";

    printf("single get : a80: %s \n", static_cast<std::string>(lw["a80"]).c_str());
    printf("single get : a81: %s \n", static_cast<std::string>(lw["a81"]).c_str());
    printf("single exist : a81: %d \n", static_cast<bool>(lw["a81"]));

    lw.start_with("a", test_case_leveldb_wr_key_value_printer, &i);

    printf("destroy status : %d \n", ldb_wr::destroy(ddir));

    printf("is closed ? %d \n", lw.is_closed() ? 1 : 0);
    lw.close();
    return 0;
}

#endif  // ARGCV_TEST_WRAPPER_LEVELDB_WR_TEST_HH