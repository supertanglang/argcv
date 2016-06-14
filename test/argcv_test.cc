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
 * The above copyright notice and this permission notice shall be included in
 *all
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
#include "argcv_test.h"

#include <cstdio>
#include <cstring>

#include "argcv/argcv.hh"
#include "concurrent/threads_test.hh"
#include "ir/index/analyzer/analyzer_test.hh"
#include "string/string_test.hh"
#include "sys/sys_test.hh"
#include "wrapper/ini_wr_test.hh"
#include "wrapper/leveldb_wr_test.hh"

static int test_case_info(int argc, char* argv[]) {
  argcv::argcv_info info;
  return 0;
}

const testcase_t test_list[] = {
    test_case_info,       test_case_string_split, test_case_string_replace,
    test_case_uuid,       test_case_stemmer,      test_case_analyzer_basic,
    test_case_leveldb_wr, test_case_ini_wr,       test_case_threads,
    test_case_dir_trav};

int main(int argc, char* argv[]) {
  TEST_CASE_EXPECT_GT(argc, 1);
  int testmax = sizeof(test_list) / sizeof(test_list[0]);
  int offset = atoi(argv[1]);
  TEST_CASE_EXPECT_LT(offset, testmax);
  return test_list[offset](argc, argv);
}