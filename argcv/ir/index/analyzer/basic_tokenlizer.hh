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
#ifndef ARGCV_IR_INDEX_ANALYZER_BASIC_TOKENLIZER_HH
#define ARGCV_IR_INDEX_ANALYZER_BASIC_TOKENLIZER_HH

#include <string>
#include <vector>
#include "tokenlizer.hh"

#include "argcv/string/string.hh"
#include "argcv/string/chars.h"

namespace argcv {
namespace ir {
namespace index {
namespace analyzer {
class basic_tokenlizer : public tokenlizer {
public:
    basic_tokenlizer(const std::string& s)
        : tokenlizer(s), data(argcv::string::stemm_s(s)), offset(0), size(data.size()) {}

    bool prev(std::string& t) { return begin() ? false : (t.assign(data[offset - 1]), true); }
    bool next(std::string& t) { return end() ? false : (t.assign(data[offset]), offset++, true); }
    bool curr(std::string& t) { return end() ? false : (t.assign(data[offset]), true); }
    bool reset() { return (offset = 0, true); }
    bool end() { return offset >= size; }
    bool begin() { return offset == 0; }

protected:
    std::vector<std::string> data;
    size_t offset;
    size_t size;
};
}
}
}
}  // namespace argcv::ir::index::analyzer

#endif  //  ARGCV_IR_INDEX_ANALYZER_BASIC_TOKENLIZER_HH
