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
#ifndef ARGCV_IR_INDEX_UTIL_HH
#define ARGCV_IR_INDEX_UTIL_HH

#include <string>

namespace argcv {
namespace ir {
namespace index {

const static std::string DB_SEPARATOR("\0", 1);
// const static std::string DB_SEPARATOR("#", 1);

inline std::string index_db_doc_prefix() {
    std::string _prefix("D");  // shall I use 'static std::string' ?
    return _prefix;
}

inline std::string index_db_idx_prefix() {
    std::string _prefix("I");
    return _prefix;
}

const static std::string DB_DOC_PREFIX(index_db_doc_prefix());
const static std::string DB_IDX_PREFIX(index_db_idx_prefix());

inline std::string index_db_meta_prefix() {
    std::string _prefix("M");
    return _prefix;
}

const static std::string DB_META_PREFIX(index_db_meta_prefix());
}
}
}  // namespace argcv::ir::index

#endif  //  ARGCV_IR_INDEX_UTIL_HH