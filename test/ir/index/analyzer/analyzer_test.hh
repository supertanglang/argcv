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
#ifndef ARGCV_TEST_IR_INDEX_ANALYZER_ANALYZER_TEST_HH
#define ARGCV_TEST_IR_INDEX_ANALYZER_ANALYZER_TEST_HH
#include "test/argcv_test.h"

#include <map>
#include <string>

#include "argcv/ir/ir.hh"
#include "argcv/ir/index/index.hh"
#include "argcv/ir/index/analyzer/basic_analyzer.hh"
#include "argcv/ir/index/analyzer/basic_tokenlizer.hh"
#include "argcv/ir/index/analyzer/util.hh"

#include "argcv/string/string.hh"

using namespace argcv::string;
using namespace argcv::ir::index::analyzer;

static int test_case_analyzer_basic(int argc, char* argv[]) {
    std::string s(
        "Computer science is the scientific and practical approach to computation and its applications. It "
        "is the systematic study of the feasibility, structure, expression, and mechanization of the "
        "methodical procedures (or algorithms) that underlie the acquisition, representation, processing, "
        "storage, communication of, and access to information, whether such information is encoded as bits "
        "in a computer memory or transcribed in genes and protein structures in a biological cell. An "
        "alternate, more succinct definition of computer science is the study of automating algorithmic "
        "processes that scale. A computer scientist specializes in the theory of computation and the design "
        "of computational systems.");
    basic_tokenlizer t(s);
    basic_analyzer a(&t);
    std::map<std::string, uint64_t> stats_c(token_stat_c(&a));

    for (std::map<std::string, uint64_t>::const_iterator it = stats_c.begin(); it != stats_c.end(); it++) {
        printf("key: %s value: %lu\n", it->first.c_str(), (unsigned long)it->second);
    }
    
    std::map<std::string, std::pair<uint64_t, std::vector<uint32_t>>> stats(token_stat(&a));
    for (std::map<std::string, std::pair<uint64_t, std::vector<uint32_t>>>::const_iterator it = stats.begin(); it != stats.end(); it++) {
        printf("term: %s count : %lu  => ", it->first.c_str(), (unsigned long)it->second.first);
        
        for(std::vector<uint32_t>::const_iterator it2 = it->second.second.begin() ; it2 != it->second.second.end() ; it2 ++) {
            printf(" [%lu]", (unsigned long)(*it2));
        }
        std::string ss = as_str<uint32_t>(it->second.second);
        
        std::vector<uint32_t>  ssv = as_vec<uint32_t>(ss);
        
        for(std::vector<uint32_t>::const_iterator it2 = ssv.begin() ; it2 != ssv.end() ; it2 ++) {
            printf(" [%lu]", (unsigned long)(*it2));
        }
        
        printf("\n");
        
    }
    return 0;
}

#endif  // ARGCV_TEST_IR_INDEX_ANALYZER_ANALYZER_TEST_HH