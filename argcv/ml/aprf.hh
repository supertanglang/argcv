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
#ifndef ARGCV_ML_APRF_HH
#define ARGCV_ML_APRF_HH

// accuracy,precision,recall and f1_measure

#include <cstdint>  // uint64_t

#include <string>
#include <utility>  // std::pair, std::make_pair
#include <vector>

#include "ml.hh"

namespace argcv {
namespace ml {
class aprf {
public:
    aprf() : tp(0), tn(0), fp(0), fn(0) {}
    ~aprf() {}

    void add(bool predicted, bool actual) {
        if (predicted && actual)
            tp++;
        else if (!predicted && actual)
            fn++;
        else if (predicted && !actual)
            fp++;
        else if (!predicted && !actual)
            tn++;
    }

    const double accuracy() const {
        size_t all = tp + fp + tn + fn;
        if (all == 0) return 0;
        return (double)(tp + tn) / all;
    }

    const double precision() const {
        if (tp + fp == 0)
            return 0;
        else
            return (double)tp / (tp + fp);
    }

    const double recall() const {
        if (tp + fn == 0)
            return 0;
        else
            return (double)tp / (tp + fn);
    }

    const double f1() const {
        // double p = precision();
        // double r = recall();
        // if (p + r == 0) return 0;
        // return 2 * p * r / (p + r);
        return f(1);
    }

    const double f(size_t a) const {
        if (a < 1) return 0;
        double p = precision();
        double r = recall();
        if (p + r == 0) return 0;
        double a2 = a * a;
        return (a2 + 1) * p * r / (a2 * (p + r));
    }

    void reset() { tp = tn = fp = fn = 0; }

private:
    size_t tp;
    size_t tn;
    size_t fp;
    size_t fn;
};
}
}  // namespace argcv::ml

#endif  //  ARGCV_ML_APRF_HH