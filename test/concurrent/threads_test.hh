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
#ifndef ARGCV_TEST_THREAD_TH_LACUS_TEST_HH
#define ARGCV_TEST_THREAD_TH_LACUS_TEST_HH

#include <unistd.h>
#include "test/argcv_test.h"

#include "argcv/concurrent/thread_pool.hh"

using namespace argcv::concurrent;

static int test_case_threads(int argc, char* argv[]) {
    size_t thread_size = 3;
    size_t task_size = 100;
    thread_pool pool(thread_size);
    bool all_done = true;
    std::vector<std::future<bool> > results;
    for (size_t i = 0; i < task_size; i++) {
        results.emplace_back(pool.enqueue([i, thread_size, task_size] {
            printf("[test_case_threads] start: %zu of %zu\n", i, task_size);
            // usleep(i);
            printf("[test_case_threads] end: %zu of %zu\n", i, task_size);
            return true;
        }));
    }

    for (auto&& result : results) {
        bool c_status = static_cast<bool>(result.get());
        if (!c_status) {
            all_done = false;
        }
    }
    return all_done ? 0 : -1;
}

#endif  // ARGCV_TEST_THREAD_TH_LACUS_TEST_HH