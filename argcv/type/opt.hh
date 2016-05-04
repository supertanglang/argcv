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
#ifndef ARGCV_TYPE_OPT_HH
#define ARGCV_TYPE_OPT_HH

namespace argcv {
namespace type {

template <typename T>
class opt {
public:
    opt() : opt<T>(new T) {}

    opt(T *_t) : _t(_t) { _ref = new opt_ref; }

    opt(const opt &o) : _t(o._t), _ref(o._ref) {
        // printf("copy(1) %s:%d\n", __FILE__, __LINE__);
        _ref->cnt++;
    }

    opt &operator=(opt &o) {
        printf("copy(2) %s:%d\n", __FILE__, __LINE__);
        if (_t == o._t) return *this;
        clean();
        _t = o._t;
        _ref = o._ref;
        _ref->cnt++;
        return *this;
    }

    template <typename K>
    opt(const opt<K> &o)
        : _t(dynamic_cast<T *>(o._t)), _ref(o._ref) {
        // printf("copy(3) %s:%d\n", __FILE__, __LINE__);
        _ref->cnt++;
    }

    template <typename K>
    opt<T> &operator=(opt<K> &o) {
        // printf("copy(4) %s:%d\n", __FILE__, __LINE__);
        if (_t == o._t) return *this;
        clean();
        _t = dynamic_cast<T *>(o._t);
        _ref = o._ref;
        _ref->cnt++;
        return *this;
    }

    ~opt() { clean(); }

    bool is_def() { return _t != nullptr; }
    bool is_empty() { return _t == nullptr; }

    operator bool() { return is_def(); }

    T &get() { return *_t; }
    T &get_or(T &_or) { return is_def() ? *_t : _or; }

    T *_data() { return _t; }
    size_t nref() { return _ref->cnt; }

    class opt_ref {
    public:
        opt_ref() { cnt = 1; }
        size_t cnt;
    };

public:
    T *_t;
    opt_ref *_ref;

    void clean() {
        _ref->cnt--;
        // printf("del(1) %s:%d\n", __FILE__, __LINE__);
        if (_ref->cnt == 0) {
            // printf("del(2) %s:%d\n", __FILE__, __LINE__);
            delete _ref;
            if (is_def()) delete _t;
        }
    }
};
}
}

#endif  //  ARGCV_TYPE_OPT_HH