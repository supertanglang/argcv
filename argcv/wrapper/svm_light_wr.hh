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
#ifndef ARGCV_WRAPPER_SVM_LIGHT_WR_HH
#define ARGCV_WRAPPER_SVM_LIGHT_WR_HH

// Support Vector Machine
//
// Progress:
// NOT STARTED
// ref: http://blog.pluskid.org/?p=632

#include <cstdint>  // uint64_t
#include <cstdio>

#include <map>
#include <string>
#include <utility>  // std::pair, std::make_pair
#include <vector>

#include "argcv/ml/ml.hh"

namespace argcv {
namespace wrapper {
namespace svm_light_wr {

using namespace ::argcv::ml;

class svm_light_wr_data;

class svm_light_wr : public ::argcv::ml::ml<double, double> {
public:
    svm_light_wr();
    svm_light_wr(dataset<double, double> data);
    virtual ~svm_light_wr();
    
    void add(std::vector<double> x, double y) { data.add(x, y); }

    bool learn();

    bool save(const std::string &path);

    bool load(const std::string &path);

    double predict(std::vector<double> x);

    bool opt(const std::string &key, const std::string &value);
    bool opt(const std::string &key, uint64_t value);
    bool opt(const std::string &key, double value);
    bool opt(const std::string &key, bool value);

    virtual void init(dataset<double, double> d);

private:
    dataset<double, double> data;
    void free_all();
    svm_light_wr_data * _sl_info;
};
}
}
}  // namespace argcv::wrapper::svm_light_wr

#endif  //  ARGCV_WRAPPER_SVM_LIGHT_WR_HH
