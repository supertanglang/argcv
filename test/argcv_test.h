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
#ifndef ARGCV_TEST_ARGCV_TEST_HH
#define ARGCV_TEST_ARGCV_TEST_HH

#include <assert.h>

typedef int (*testcase_t)(int, char* []);

#define TEST_CASE_EXPECT_TRUE(a) assert(a != 0)
#define TEST_CASE_EXPECT_FALSE(a) assert(a == 0)
#define TEST_CASE_EXPECT_EQ(a, b) assert(a == b)
#define TEST_CASE_EXPECT_NE(a, b) assert(a == b)
#define TEST_CASE_EXPECT_LT(a, b) assert(a < b)
#define TEST_CASE_EXPECT_LE(a, b) assert(a <= b)
#define TEST_CASE_EXPECT_GT(a, b) assert(a > b)
#define TEST_CASE_EXPECT_GE(a, b) assert(a >= b)


#endif  // ARGCV_TEST_ARGCV_TEST_HH