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
#ifndef ARGCV_IR_INDEX_DOC_HH
#define ARGCV_IR_INDEX_DOC_HH

#include <string>
#include <vector>

#include "field.hh"
#include "util.hh"
#include "argcv/string/string.hh"
#include "argcv/string/uuid.hh"

namespace argcv {
namespace ir {
namespace index {

using ::argcv::string::as_type;
using ::argcv::string::as_vec;
using ::argcv::string::split;
using ::argcv::string::uuid;

class doc {
public:
    doc(const std::string& k, const std::string& v, size_t sz_prefix) {
        std::string pk = k.substr(sz_prefix);
        std::vector<std::string> _kvec = split(pk, DB_SEPARATOR);
        size_t pos_uuid = pk.length() - sizeof(uint64_t) * 2;
        size_t pos_sz = pos_uuid - sizeof(size_t);
        _id = pk.substr(pos_uuid, sizeof(uint64_t) * 2);
        _term = _kvec[0];
        _sz = ~as_type<size_t>(pk, pos_sz);
        _vpos = as_vec<size_t>(v);
        _score = 0;
    }

    const std::string id() const { return _id; }

    const std::string id_s() const {
        uuid uid(_id);
        return uid.str();
    }
    const std::string term() const { return _term; }

    const size_t size() const { return _sz; }

    const std::vector<size_t>& vpos() const { return _vpos; }

    double& score() { return _score; }

protected:
    std::string _id;            // binary id
    std::string _term;          // term .. is it necessary ?
    size_t _sz;                 //
    std::vector<size_t> _vpos;  // position
    double _score;
};

class docs {
public:
    std::vector<doc> data;
    size_t sz_prefix;
    size_t sz_limit;
    size_t sz_data;
};
}
}
}  // namespace argcv::ir::index

#endif  //  ARGCV_IR_INDEX_DOC_HH