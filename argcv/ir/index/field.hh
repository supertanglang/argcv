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
#ifndef ARGCV_IR_INDEX_FIELD_HH
#define ARGCV_IR_INDEX_FIELD_HH

#include <map>
#include <set>
#include <string>
#include <utility>  // std::pair
#include <vector>

#include "analyzer/analyzer.hh"
#include "argcv/string/string.hh"
#include "util.hh"

namespace argcv {
namespace ir {
namespace index {

using ::argcv::string::as_str;

template <typename T>
class field {
public:
    field() {}

    virtual ~field() {}

    virtual size_t size() {
        size_t _size = 0;
        for (std::map<std::string, std::vector<size_t>>::const_iterator it = _key_vpos.begin();
             it != _key_vpos.end(); it++) {
            _size += it->second.size();
        }
        return _size;
    }

    virtual std::vector<std::string> keys(const std::string& id) {
        std::vector<std::string> _keys;
        for (std::map<std::string, std::vector<size_t>>::const_iterator it = _key_vpos.begin();
             it != _key_vpos.end(); it++) {
            std::string _k = it->first;
            _k += DB_SEPARATOR;
            _k += as_str<size_t>(~(size_t)(it->second.size()));
            _k += id;
            _keys.push_back(_k);
        }
        return _keys;
    }

    // keys are parsed doc segs, values are position of all these terms
    virtual std::map<std::string, std::vector<size_t>> key_vpos() { return _key_vpos; }

    virtual std::vector<std::pair<std::string, std::string>> key_values(const std::string& id) {
        std::vector<std::pair<std::string, std::string>> _key_values;
        for (std::map<std::string, std::vector<size_t>>::const_iterator it = _key_vpos.begin();
             it != _key_vpos.end(); it++) {
            if (it->second.size() > 0) {
                std::string _k = it->first;
                _k += DB_SEPARATOR;
                _k += as_str<size_t>(~(size_t)(it->second.size()));
                _k += id;
                _key_values.push_back(std::make_pair(_k, as_str<size_t>(it->second)));
            }
        }
        return _key_values;
    }

protected:
    std::map<std::string, std::vector<size_t>> _key_vpos;
};

template <typename T>
class number_field : public field<T> {
public:
    number_field(T d) : field<T>() {
        std::string str;
        str += as_str<T>(d);
        std::vector<size_t> pos;
        pos.push_back(0);
        this->_key_vpos[str] = pos;
    }

protected:
    number_field() : field<T>() {}
};

typedef number_field<int64_t> long_field;
typedef number_field<double> double_field;

class string_field : public field<std::string> {
public:
    string_field(const std::string& str) : field<std::string>() {
        std::vector<size_t> pos;
        pos.push_back(0);
        _key_vpos[str] = pos;
    }

protected:
    string_field() : field<std::string>() {}  // do nothing, for extends
};

class text_field : public string_field {
public:
    text_field(analyzer::analyzer& anz) : string_field() {
        anz.reset();
        std::string token;
        size_t pos = 0;
        while (anz.next(token)) {
            _key_vpos[token].push_back(pos);
            pos++;
        }
    }
};

class vector_text_field : public field<std::string> {
public:
    vector_text_field(std::vector<analyzer::analyzer> vec, size_t dists) : field<std::string>() {
        size_t pos = 0;
        for (std::vector<analyzer::analyzer>::iterator it = vec.begin(); it != vec.end(); it++) {
            it->reset();
            std::string token;
            while (it->next(token)) {
                _key_vpos[token].push_back(pos);
                pos++;
            }
            pos += dists - 1;
        }
    }
};
}
}
}  // namespace argcv::ir::index

#endif  //  ARGCV_IR_INDEX_FIELD_HH