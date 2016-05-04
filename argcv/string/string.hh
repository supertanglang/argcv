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
#ifndef ARGCV_STRING_STRING_HH
#define ARGCV_STRING_STRING_HH

#include <cstring>

#include <algorithm>
#include <string>
#include <vector>

namespace argcv {
namespace string {

inline std::vector<std::string> &split(const std::string &s, const std::string &delim,
                                       std::vector<std::string> *_elems, bool skip_blank_elem = true) {
    std::string item;
    if (delim.empty()) {
        _elems->push_back(s);
        return *_elems;
    }
    std::string::const_iterator substart = s.begin(), subend;
    while (true) {
        subend = search(substart, s.end(), delim.begin(), delim.end());
        std::string temp(substart, subend);
        if (!skip_blank_elem || !temp.empty()) {
            _elems->push_back(temp);
        }
        if (subend == s.end()) {
            break;
        }
        substart = subend + delim.size();
    }
    return *_elems;
}

inline std::vector<std::string> split(const std::string &s, const std::string &delim,
                                      bool skip_blank_elem = true) {
    std::vector<std::string> elems;
    split(s, delim, &elems, skip_blank_elem);
    return elems;
}

inline std::string replace(std::string &str_raw, const std::string &str_src, const std::string &str_dst) {
    std::string::size_type pos = 0;
    std::string::size_type src_len = str_src.size();
    std::string::size_type dst_len = str_dst.size();
    while ((pos = str_raw.find(str_src, pos)) != std::string::npos) {
        str_raw.replace(pos, src_len, str_dst);
        pos += dst_len;
    }
    return str_raw;
}

template <typename T>
std::string as_str(T v) {
    return std::string((char *)&v, sizeof(T));
}

template <typename T>
std::string as_str(std::vector<T> v) {
    std::string r;
    for (size_t ix = 0; ix < v.size(); ix++) {
        r += std::string((char *)(&(v[ix])), sizeof(T));
    }
    return r;
}

template <typename T>
T as_type(const std::string s) {
    T v;
    size_t sz = sizeof(T);
    if (s.size() < sz) {
        memset(&v, 0, sz);
    } else {
        memcpy(&v, s.data(), sz);
    }
    return v;
}

template <typename T>
T as_type(const std::string s, size_t offset) {
    T v;
    size_t sz = sizeof(T);
    if (s.size() < sz + offset) {
        memset(&v, 0, sz);
    } else {
        memcpy(&v, s.data() + offset, sz);
    }
    return v;
}

template <typename T>
std::vector<T> as_vec(const std::string s, size_t offset = 0) {
    std::vector<T> v;
    size_t tsz = sizeof(T);
    size_t sz = (s.size() - offset) / tsz;
    for (size_t ix = 0; ix < sz; ix++) {
        T tv;
        memcpy(&tv, ((char *)s.data()) + ix * tsz + offset, tsz);
        v.push_back(tv);
    }
    return v;
}

inline int utf8_char_length(unsigned char ch) {
    unsigned char t = 0x80;
    if (!(ch & t))  // ! 1000 0000 B
        return 1;
    t = t >> 1;
    if (!(ch & t))  // ! 0100 0000 B
        return 0;
    t = t >> 1;
    int l = 2;
    while (ch & t) {
        t = t >> 1;
        l++;
        if (l > 6) return 0;
    }
    return l;
}

inline std::vector<std::string> utf8split(const std::string &s) {
    std::vector<std::string> elems;
    size_t clen = 0;
    for (size_t i = 0; i < s.length(); i++) {
        if ((clen = utf8_char_length(s[i]))) {
            elems.push_back(s.substr(i, clen));
            i += clen - 1;
        } else {
            // fprintf(stderr, "error size: %zu %zu\n", clen, i);
            break;
        }
    }
    return elems;
}

/**
 * stemm a string
 */
std::vector<std::string> stemm_s(const std::string &str);

/**
 * stemm a string, it will remove all Non ASCII words
 */
std::vector<std::string> stemm_s_cl(const std::string &str);

/**
 * stemm a word
 */
std::string stemm_w(const std::string &word);
}
}  // argcv::string

#endif  //  ARGCV_STRING_STRING_HH
