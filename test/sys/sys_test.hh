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
#ifndef ARGCV_TEST_ARGCV_SYS_SYS_TEST_HH
#define ARGCV_TEST_ARGCV_SYS_SYS_TEST_HH
#include "../argcv_test.h"

#include <stdio.h>

#include <string>

#include "argcv/sys/dirs.h"

using namespace argcv::string;

int dir_trav(const char* path, int recu, int (*file_handle)(const char* file_name, int is_dir, void* user),
             void* user);

int test_case_dir_trav_handler(const char* file_name, int is_dir, void* user) {
    int* u_data = (int*)user;
    printf("path:%s is_dir:%s data:%d\n", file_name, is_dir ? "yes" : "no", *u_data);
    (*u_data)++;
    return !((*u_data) < 10);
}

static int test_case_dir_trav(int argc, char* argv[]) {
    printf("[test_case_dir_trav] start\n");
    //std::string path("/tmp");
    std::string path(".");
    int u_data = 0;
    dir_trav(path.c_str(),1,test_case_dir_trav_handler,&u_data);
    printf("[test_case_dir_trav] end\n");
    return 0;
}

#endif  // ARGCV_TEST_ARGCV_SYS_SYS_TEST_HH