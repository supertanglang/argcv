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
#ifndef ARGCV_IR_IR_HH
#define ARGCV_IR_IR_HH

// Information retrieval (IR) is the activity of obtaining information
// resources relevant to an information need from a collection of
// information resources. Searches can be based on metadata or
// on full-text (or other content-based) indexing.
// ref: http://en.wikipedia.org/wiki/Information_retrieval

#include <cmath>

namespace argcv {
namespace ir {

// tf-idf
// assume : k in document D
// stid : size of term k in  D
// atsid :  all term size in D
// ads : all document size
// dscct : document size contains current term
inline double tf_idf(size_t stid, size_t atsid, size_t ads, size_t dscct) {
    // #define MATH_LG_10 2.302585
    // tf * idf
    if (ads == 0 || atsid == 0 || dscct == 0) return 0;
    return (static_cast<double>(stid) / atsid) * log(static_cast<double>(ads) / (dscct)) / 2.302585;
}
}
}  // namespace argcv::ir

#endif  //  ARGCV_IR_IR_HH
