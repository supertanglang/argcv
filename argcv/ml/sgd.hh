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
#ifndef ARGCV_ML_SGD_HH
#define ARGCV_ML_SGD_HH

// Gradient Descent
// Gradient Descent is a first-order optimization algorithm.
// To find a local minimum of a function using gradient descent,
// one takes steps proportional to the negative of the gradient
// (or of the approximate gradient) of the function at the
// current point.

// Stochastic Gradient Descent
// Stochastic gradient descent is a gradient descent optimization
// method for minimizing an objective function that is written as
// a sum of differentiable functions.

#include <cmath>
#include <cstdint>  // uint64_t
#include <cstdio>

#include <map>
#include <string>
#include <utility>  // std::pair, std::make_pair
#include <vector>

#include "ml.hh"

namespace argcv {
namespace ml {

class sgd : public ml<double, double> {
public:
    sgd(double delta = 0.001, double e = 0.00001, size_t loop_limit = 0, double theta_limit = 1000)
        : data(dataset<double, double>()),
          delta(delta),
          e(e),
          loop_limit(loop_limit),
          theta_limit(theta_limit) {
        printf("sgd() starting ..\n");
    }

    sgd(dataset<double, double> data, double delta = 0.001, double e = 0.00001, size_t loop_limit = 0,
        double theta_limit = 1000)
        : data(data), delta(delta), e(e), loop_limit(loop_limit), theta_limit(theta_limit) {
        printf("sgd(dataset_dd) starting ..\n");
    }

    ~sgd() { printf("sgd finished ..\n"); }

    void add(std::vector<double> x, double y) { data.add(x, y); }

    bool learn() {
        printf("size of dataset: %llu \n", data.size());
        if (data.size() < 1) {
            fprintf(stderr, "size of dataset is ZERO, return failed\n");
            return false;  // dataset not found.
        }
        sz_w = data.x_at(0).size();
        for (size_t ix = 0; ix < sz_w; ix++) wx.push_back(0.0);
        b = 0.0;
        double ls_cloop;
        size_t loop = 0;
        uint64_t sz_data = data.size();

        int oscillation_check[3] = {0, 0, 0};
        size_t oscillation_check_point = 0;
        double min_delta = e / 10;
        while ((ls_cloop = la(wx, b)) > e && (loop_limit == 0 || loop < loop_limit)) {
            loop++;
            if (loop % 1 == 0) {
                print_ws(false);
                printf(" e = %.6f  lose = %f  [%zu]\n", e, ls_cloop, loop);
            }
            for (uint64_t i = 0; i < sz_data; i++) {
                double fx = f(i, wx, b);
                double theta = (data.y_at(i) - fx);
                if (theta > theta_limit) {
                    theta = theta_limit;
                } else if (theta < -theta_limit) {
                    theta = -theta_limit;
                }
                oscillation_check_point = (oscillation_check_point + 1) % 3;
                oscillation_check[oscillation_check_point] = theta > 0 ? 1 : -1;
                size_t ocp = 0;
                size_t ocn = 0;
                for (size_t oci = 0; oci < 3; oci++) {
                    switch (oscillation_check[oci]) {
                        case 1:
                            ocp++;
                            break;
                        case -1:
                            ocn++;
                            break;
                    }
                }
                if ((ocp == 2 && ocn == 1) || (ocp == 1 && ocn == 2)) {
                    // in oscillation
                    // printf("-- delta : %.16f => %.16f \n", delta, delta / 2 + min_delta);
                    delta = delta / 2 + min_delta;
                } else if (ocp == 3 || ocn == 3) {
                    // printf("++ delta : %.16f => %.16f \n", delta, 2 * delta);
                    delta *= 2;
                }

                for (size_t j = 0; j < sz_w; j++) {
                    double vev = delta * theta * data.x_at(i, j);
                    // printf("(%llu,%zu)  (%.2f,  %.2f) v %.4f\n", i, j, theta, data.x_at(i, j), vev);
                    wx[j] += vev;
                    // wx[j] += delta * (data.y_at(i) - fx) * data.x_at(i, j);
                }
                b += delta * theta;
            }
        }

        printf("jump out , finally : \n");
        print_ws(false);
        printf(" e = %.6f  lose = %f  [%zu]\n", e, ls_cloop, loop);
        return true;
    }

    bool save(const std::string &path) { return false; }

    bool load(const std::string &path) { return false; }

    double predict(std::vector<double> x) {
        double fx = 0;
        for (size_t i = 0; i < wx.size(); i++) {
            fx += x[i] * wx[i];
        }
        return fx + b;
    }

private:
    dataset<double, double> data;
    double delta;
    double e;
    size_t loop_limit;
    double theta_limit;
    size_t sz_w;
    std::vector<double> wx;
    double b;  // wx * x + b

    void print_ws(int endl = false) {
        for (size_t i = 0; i < wx.size(); i++) {
            printf("%.10f * x[%zu] +", wx[i], i);
        }
        printf("%.10f ", b);
        if (endl) printf("\n");
    }

    /**
     * collect all lost
     */
    double la(const std::vector<double> &wx, const double b) {
        double sum = 0;
        uint64_t sz_data = data.size();
        for (uint64_t i = 0; i < sz_data; i++) {
            sum += l(i, wx, b, data.y_at(i));
        }
        return sum / sz_data;
    }

    // lost
    double l(uint64_t ix, const std::vector<double> &wx, const double b, double y) {
        double fx = f(ix, wx, b);
        double dfx = fx - y;
        return dfx * dfx / 2;
    }

    // function
    double f(uint64_t ix, const std::vector<double> &wx, const double b) {
        double fx = 0;
        for (size_t i = 0; i < wx.size(); i++) {
            // fx += pow(data.x_at(ix, i), i + 1) * wx[i];
            fx += data.x_at(ix, i) * wx[i];
        }
        return fx + b;
    }
};
}
}  // namespace argcv::ml

#endif  //  ARGCV_ML_SGD_HH
