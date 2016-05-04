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
#ifndef ARGCV_TEST_WRAPPER_INI_WR_TEST_HH
#define ARGCV_TEST_WRAPPER_INI_WR_TEST_HH
#include "test/argcv_test.h"

#include <map>
#include <set>
#include <string>

#include "argcv/wrapper/ini_wr.hh"
using namespace argcv::string;
using namespace argcv::wrapper::ini;


static int test_case_ini_wr(int argc, char* argv[]) {
    ini_wr iw("config.ini");
    iw["a"]["0"] = 3.0;
    printf("value : %s \n",static_cast<std::string>(iw["a"]["0"]).c_str());
    printf("s1n1 : %s \n",static_cast<std::string>(iw["s1"]["n1"]).c_str());
    printf("s2n1 : %s \n",static_cast<std::string>(iw["s2"]["n1"]).c_str());
    printf("s2n2 : %s \n",static_cast<std::string>(iw["s2"]["n2"]).c_str());
    return 0;
}

#endif  // ARGCV_TEST_WRAPPER_INI_WR_TEST_HH