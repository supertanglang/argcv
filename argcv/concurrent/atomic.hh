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
#ifndef ARGCV_CONCURRENT_THREADS_HH
#define ARGCV_CONCURRENT_THREADS_HH

#include <atomic>

namespace argcv {
namespace concurrent {

// http://en.cppreference.com/w/cpp/language/type_alias
template <typename T>
using atomic = std::atomic<T>;  // typedef std::atomic<T> as atomic<T>

template <typename T>
T* atomic_fetch_add(atomic<T*>* obj, std::ptrdiff_t arg) {
    return obj->fetch_add(arg);
}

template <typename T>
T* atomic_fetch_sub(atomic<T*>* obj, std::ptrdiff_t arg) {
    return obj->fetch_sub(arg);
}

template <typename T>
T* atomic_fetch_or(atomic<T*>* obj, std::ptrdiff_t arg) {
    return obj->fetch_or(arg);
}


template <typename T>
T* atomic_fetch_xor(atomic<T*>* obj, std::ptrdiff_t arg) {
    return obj->fetch_xor(arg);
}



    
    
}
}  // argcv::concurrent

#endif  //  ARGCV_CONCURRENT_THREADS_HH
