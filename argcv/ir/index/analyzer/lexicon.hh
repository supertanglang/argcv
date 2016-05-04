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
#ifndef ARGCV_IR_INDEX_ANALYZER_LEXICON_HH
#define ARGCV_IR_INDEX_ANALYZER_LEXICON_HH

#include <cstdio>
#include <cstring>

#include <string>
#include <vector>

#include "argcv/string/string.hh"
#include "argcv/wrapper/leveldb_wr.hh"

namespace argcv {
namespace ir {
namespace index {
namespace analyzer {
using ::argcv::wrapper::leveldb::ldb_wr;
using ::argcv::string::as_type;
using ::argcv::string::as_str;
using ::argcv::string::split;

class lexicon {
public:
    lexicon(const std::string& dpath, size_t cache_size = 0) : db(ldb_wr(dpath, cache_size)) { db.conn(); }
    ~lexicon() { db.close(); }

    void set(const std::string& term, size_t sz = 0) { db.put(term, as_str<size_t>(sz)); }
    void unset(const std::string& term) { db.rm(term); }
    size_t count(const std::string& term) {
        std::string rt;
        db.get(term, &rt);
        return as_type<size_t>(rt);
    }

    size_t level(const std::string& term) {
        size_t c = count(term);
        size_t lv = 0;
        while (c > 0) {
            lv++;
            c = c >> 4;
        }
        return lv;
    }

    // load files from https://github.com/ling0322/webdict/tree/master
    size_t load(const std::string& path) {
        size_t sz = 0;
        FILE* fp = fopen(path.c_str(), "r");
        if (fp == nullptr) {  // file not found
            return sz;
        }
        char buff[1024];
        memset(buff, 0, sizeof(char) * 1024);
        std::vector<std::string> line;
        while (fgets(buff, 1023, fp) != nullptr) {
            line.clear();
            split(buff, " ", &line);
            if (line.size() == 2) {
                int fq = std::stoi(line[1]);
                if (fq > 0) {
                    sz++;
                    set(line[0], (size_t)fq);
                }
            }
        }
        fclose(fp);
        return sz;
    }

private:
    ldb_wr db;
};
}
}
}
}  // namespace argcv::ir::index::analyzer

#endif  //  ARGCV_IR_INDEX_ANALYZER_LEXICON_HH
