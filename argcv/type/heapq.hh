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
#ifndef ARGCV_TYPE_HEAPQ_HH
#define ARGCV_TYPE_HEAPQ_HH

namespace argcv {
namespace type {

template <typename T>
class heapq {
private:
    int size_limit;
    int size;
    int (*compare)(T, T);
    T* data;

public:
    heapq(int size_limit, int (*compare)(T, T)) : size_limit(size_limit), size(0), compare(compare) {
        data = new T[size_limit + 1];
    }

    ~heapq() { delete[] data; }

    void adjust() {
        int pos = (size - 2) / 2;
        int csize = size;
        while (pos >= 0) {
            sift_down(pos, csize - 1);
            pos--;
        }
    }

    void sift_down(int i, int m) {
        T t = data[i];  // T
        for (int j = 2 * i + 1; j <= m; j = 2 * j + 1) {
            if (j < m && compare(data[j], data[j + 1]) > 0) {
                j++;
            }
            if (compare(t, data[j]) <= 0) {
                break;
            } else {
                // data.set(i, data.get(j));
                data[i] = data[j];
                i = j;
            }
        }
        // data.set(i, t);
        data[i] = t;
    }

    void sift_up(int start) {
        T t = data[start];  // T
        int j = start;
        int i = (j - 1) / 2;
        while (j > 0) {
            if (compare(data[i], t) <= 0) {
                break;
            } else {
                data[j] = data[i];
                j = i;
                i = (i - 1) / 2;
            }
        }
        data[j] = t;
    }

    int get_size() { return size; }

    int get_size_limit() { return size_limit; }

    void set_size_limit(int my_size_limit) { size_limit = my_size_limit <= 0 ? 0 : my_size_limit; }

    // a > b return 1;
    // a == b return 0;
    // a < b return -1;
    // virtual int compare(T t1, T t2);
    // virtual int compare(T a, T b) {
    // printf("compare : %f - %f = ? %d\n",(double)a,(double)b,(a > b ? 1 : (a == b ? 0 : -1)));
    //    return a > b ? 1 : (a == b ? 0 : -1);
    //}

    bool pop(T& val) {
        if (size == 0) return false;
        T rt = data[0];
        data[0] = data[size - 1];
        size--;
        sift_down(0, size - 1);
        val = rt;
        return true;
    }

    bool pop() {
        T val;
        return pop(val);
    }

    bool push(const T& item) {
        if (size >= size_limit) {  // to limit
                                   // pop & push
            // printf("push and pop , size : %d , size_limit : %d\n", size, size_limit);
            if (compare(item, data[0]) > 0) {
                data[size] = item;
                sift_up(size);
                size++;
                pop();
            }
        } else {  // just add it
            // printf("push size : %d , size_limit : %d\n", size, size_limit);
            data[size] = item;
            sift_up(size);
            size++;
        }
        return true;
    }

    bool empty() { return size == 0; }

    void clear() {
        delete[] data;
        size = 0;
    }
};
}
}  // namespace argcv::type::

#endif  // ARGCV_TYPE_HEAPQ_HH