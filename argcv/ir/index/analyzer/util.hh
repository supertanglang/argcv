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
#ifndef ARGCV_IR_INDEX_ANALYZER_UTIL_HH
#define ARGCV_IR_INDEX_ANALYZER_UTIL_HH

#include <map>
#include <string>   // std::string
#include <vector>   // vector
#include <utility>  // make_pair

#include "analyzer.hh"

namespace argcv {
namespace ir {
namespace index {
namespace analyzer {
/**
 * deprecated...
 */

inline std::map<std::string, std::pair<uint64_t, std::vector<uint32_t>>> token_stat(analyzer* _t) {
    std::map<std::string, std::pair<uint64_t, std::vector<uint32_t>>> s;
    std::string v;
    _t->reset();
    size_t pos = 0;
    while (_t->next(v)) {
        if (s.find(v) == s.end()) {
            std::vector<uint32_t> pl;
            pl.push_back(pos);
            s[v] = std::make_pair(1, pl);
        } else {
            s[v].first++;
            s[v].second.push_back(pos);
        }
        pos++;
    }
    return s;
}

inline std::map<std::string, uint64_t> token_stat_c(analyzer* _t) {
    std::map<std::string, uint64_t> s;
    std::string v;
    _t->reset();
    while (_t->next(v)) {
        if (s.find(v) == s.end()) {
            s[v] = 1;
        } else {
            s[v]++;
        }
    }
    return s;
}
}
}
}
}  // namespace argcv::ir::index::analyzer

#endif  //  ARGCV_IR_INDEX_ANALYZER_UTIL_HH
