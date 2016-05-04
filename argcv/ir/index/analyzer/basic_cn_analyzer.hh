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
#ifndef ARGCV_IR_INDEX_ANALYZER_BASIC_CN_ANALYZER_HH
#define ARGCV_IR_INDEX_ANALYZER_BASIC_CN_ANALYZER_HH

#include <cassert>

#include <string>
#include <set>
#include <stack>

#include "analyzer.hh"
#include "lexicon.hh"

#include "argcv/string/chars.h"
#include "argcv/string/string.hh"

namespace argcv {
namespace ir {
namespace index {
namespace analyzer {
class basic_cn_analyzer : public analyzer {
public:
    basic_cn_analyzer(tokenlizer* _t, lexicon* _lex, size_t max_word_length = 5, size_t chunk_word_size = 3)
        : analyzer(_t), max_word_length(max_word_length), chunk_word_size(chunk_word_size) {
        _t->reset();
        std::string s;
        while (_t->next(s)) {
            // if (s.length() > 0 && !is_western_punct(s[0])) {
            if (s.length() > 0) {
                if (is_western_punct(s[0])) {
                    data.push_back(s + "/ap");
                } else if (argcv::string::utf8_char_length(s[0]) == 1) {
                    data.push_back(s + "/aw");
                } else {
                    std::vector<std::string> elems = argcv::string::utf8split(s);
                    // printf("%s %d add: %s\n", __FILE__, __LINE__, s.c_str());
                    // std::vector<std::string>::size_type ij = 0;
                    std::string buff = "";
                    std::stack<std::string> rev;
                    // for (std::vector<std::string>::size_type ix = 0; ix != elems.size(); ix++) {
                    for (int ix = elems.size() - 1; ix >= 0; ix--) {
                        std::string e = elems[ix];
                        if (_is_punct(e)) {
                            if (buff.length() > 0) {
                                //data.push_back(buff + "/uw");
                                rev.push(buff + "/uw");
                                buff = "";
                            }
                            //data.push_back(e + "/up");
                            rev.push(e + "/up");
                        } else {
                            //size_t clen = buff.length();
                            //buff += e;
                            e += buff;
                            if (_lex->count(e) == 0) {
                                //buff.erase(clen);
                                if (buff.length() > 0) {
                                    //data.push_back(buff + "/uw");
                                    rev.push(buff + "/uw");
                                }
                                //buff = e;
                                buff = elems[ix];
                            } else {
                                buff = e;
                            }
                            // data.push_back( e + "/utf8_word");
                        }
                    }
                    if (buff.length() > 0) {
                        data.push_back(buff + "/uw");
                    }
                    while(!rev.empty()) {
                        data.push_back(rev.top());
                        rev.pop();
                    }
                }
            } else {
                assert(false);  // there may sth wrong if comes to here
                // printf("%s %d ignore: %s\n", __FILE__, __LINE__, s.c_str());
            }
        }
        offset = 0;
        size = data.size();
    }

    bool prev(std::string& t) { return begin() ? false : (t.assign(data[offset - 1]), true); }
    bool next(std::string& t) { return end() ? false : (t.assign(data[offset]), offset++, true); }
    bool curr(std::string& t) { return end() ? false : (t.assign(data[offset]), true); }
    bool reset() { return (offset = 0, true); }
    bool end() { return offset >= size; }
    bool begin() { return offset == 0; }

protected:
    size_t max_word_length;
    size_t chunk_word_size;
    std::vector<std::string> data;
    size_t offset;
    size_t size;

    bool _is_punct(const std::string& s) {
        static std::set<std::string> ps = _all_cn_puncts();
        if (s.length() > 0) {
            return is_western_punct(s[0]) || ps.find(s) != ps.end();
        } else {
            return false;
        }
    }

    std::set<std::string> _all_cn_puncts() {
        std::set<std::string> ps;
        ps.insert("，");
        ps.insert("！");
        ps.insert("＃");
        ps.insert("¥");
        ps.insert("％");
        ps.insert("☆");
        ps.insert("＊");
        ps.insert("（");
        ps.insert("）");
        ps.insert("－");
        ps.insert("＋");
        ps.insert("＝");
        ps.insert("｜");
        ps.insert("、");
        ps.insert("［");
        ps.insert("］");
        ps.insert("｛");
        ps.insert("｝");
        ps.insert("；");
        ps.insert("：");
        ps.insert("“");
        ps.insert("”");
        ps.insert("‘");
        ps.insert("’");
        ps.insert("《");
        ps.insert("》");
        ps.insert("，");
        ps.insert("。");
        ps.insert("／");
        ps.insert("？");
        ps.insert("【");
        ps.insert("】");
        ps.insert("〖");
        ps.insert("〗");
        ps.insert("﹝");
        ps.insert("﹞");
        ps.insert("⎡");
        ps.insert("⎤");
        return ps;
    }
};
}
}
}
}  // namespace argcv::ir::index::analyzer

#endif  //  ARGCV_IR_INDEX_ANALYZER_BASIC_CN_ANALYZER_HH
