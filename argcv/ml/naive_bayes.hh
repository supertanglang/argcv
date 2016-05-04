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
#ifndef ARGCV_ML_NAIVE_BAYES_HH
#define ARGCV_ML_NAIVE_BAYES_HH

// Perceptron
// In machine learning, the perceptron is an algorithm for supervised
// classification of an input into one of several possible non-binary outputs.

// Progress:
// TODO: load & save

#include <cstdint>  // uint64_t
#include <cstdio>

#include <map>
#include <string>
#include <utility>  // std::pair, std::make_pair
#include <vector>

#include "argcv/wrapper/leveldb_wr.hh"
#include "ml.hh"

namespace argcv {
namespace ml {

const static std::string NB_DB_SEPARATOR("\0", 1);

class naive_bayes : public ml<std::string, std::string> {
public:
    naive_bayes() {}

    naive_bayes(dataset<std::string, std::string> data) : data(data) {}

    ~naive_bayes() {}

    void add(std::string x[], int len, std::string y) { add(std::vector<std::string>(x, x + len), y); }

    void add(std::vector<std::string> x, std::string y) { data.add(x, y); }

    void add(std::pair<std::vector<std::string>, std::string> ds_item) { add(ds_item.first, ds_item.second); }

    bool learn() {
        wx.clear();
        wy.clear();
        dx_key2id.clear();
        dy_key2id.clear();
        size_t sz_data = data.size();
        size_t sz_x = data.x_size();
        for (size_t ix = 0; ix < sz_data; ix++) {
            wy[get_or_append(dy_key2id, wy, data.y_at(ix))]++;
        }

        size_t sz_cd_y = dy_key2id.size();

        for (size_t ixx = 0; ixx < sz_x; ixx++) {
            dx_key2id.push_back(std::map<std::string, int>());
        }
        for (size_t ix = 0; ix < sz_cd_y; ix++) {
            std::vector<std::vector<double>> wxx;
            for (size_t ixx = 0; ixx < sz_x; ixx++) {
                wxx.push_back(std::vector<double>());  // for each w
            }
            wx.push_back(wxx);
        }
        for (size_t ix = 0; ix < sz_data; ix++) {
            int yid = find_by_key(dy_key2id, data.y_at(ix));
            for (size_t jx = 0; jx < sz_x; jx++) {
                wx[yid][jx][get_or_append(dx_key2id[jx], wx[yid][jx], data.x_at(ix, jx))]++;
                // printf("%d-%s => %d\n",jx, data.x_at(ix,
                // jx).c_str(),find_by_key(dx_key2id[jx],data.x_at(ix, jx)));
            }
        }

        std::map<int, std::string> id2dy;
        for (std::map<std::string, int>::const_iterator it = dy_key2id.begin(); it != dy_key2id.end(); it++) {
            id2dy.insert(std::make_pair(it->second, it->first));
        }
        for (size_t iy = 0; iy < sz_cd_y; iy++) {
            for (size_t ix = 0; ix < sz_x; ix++) {
                size_t sz_cd_x = dx_key2id[ix].size();
                std::vector<double> &wxx = wx[iy][ix];
                while (wxx.size() < sz_cd_x) wxx.push_back(0);
                for (size_t ixx = 0; ixx < sz_cd_x; ixx++) {
                    /*
                    printf("set : y = %s x_%d=%d value : %d / %d (%f - %zu )   => %f \n",
                            id2dy[iy].c_str(),
                            ix,
                            ixx,
                           (int)(wxx[ixx] + 1), (int)(wy[iy] + sz_cd_x),wy[iy],sz_cd_x , (wxx[ixx] + 1) /
                    (wy[iy] + sz_cd_x));
                          */
                    wxx[ixx] = (wxx[ixx] + 1) / (wy[iy] + sz_cd_x);
                }
            }
        }

        for (std::vector<double>::size_type ix = 0; ix != sz_cd_y; ix++) {
            wy[ix] = (wy[ix] + 1) / (sz_data + sz_cd_y);
        }
        /*
        for (std::map<std::string, int>::const_iterator it = dy_key2id.begin(); it != dy_key2id.end(); it++) {
            printf("%s - %f \n", it->first.c_str(), wy[it->second]);
        }*/
        return false;
    }

    bool save(const std::string &path) {
        ::argcv::wrapper::leveldb::ldb_wr db(path);
        if (!db.conn()) {
            fprintf(stderr, "model open failed .. \n");
            return false;
        }

        db.close();
        return true;
    }

    bool load(const std::string &path) {
        ::argcv::wrapper::leveldb::ldb_wr db(path);
        if (!db.conn()) {
            fprintf(stderr, "model open failed .. \n");
            return false;
        }

        db.close();
        return true;
    }

    std::string predict(std::vector<std::string> x) {
        std::string max_label = "";
        double max_val = -1;
        for (std::map<std::string, int>::const_iterator it = dy_key2id.begin(); it != dy_key2id.end(); it++) {
            // max_label = it->first;
            double c_score = wy[it->second];
            std::vector<std::vector<double>> &wxx = wx[it->second];
            size_t sz_x = wxx.size();
            for (size_t ix = 0; ix < sz_x; ix++) {
                std::string cx = x[ix];
                if (dx_key2id[ix].find(cx) != dx_key2id[ix].end()) {
                    int nid = dx_key2id[ix].find(cx)->second;
                    double cw = wxx[ix][nid];
                    // printf("ix : %zu key: %s  w: %f \n", ix, cx.c_str(), cw);
                    c_score *= cw;
                } else {
                    c_score = 1;  // score not found ?
                    break;
                }
            }
            // printf("%s - %f \n", .c_str(), wy[it->second]);
            // printf("label : %s score : %f \n", it->first.c_str(), c_score);
            if (max_val == -1 || c_score > max_val) {
                max_val = c_score;
                max_label = it->first;
            }
        }
        return max_label;
    }

private:
    dataset<std::string, std::string> data;
    std::vector<std::vector<std::vector<double>>> wx;
    std::vector<double> wy;
    std::vector<std::map<std::string, int>> dx_key2id;
    std::map<std::string, int> dy_key2id;

    // unsafe.
    int find_by_key(const std::map<std::string, int> &key2id, const std::string &key) {
        return key2id.find(key)->second;
    }

    int get_or_append(std::map<std::string, int> &key2id, std::vector<double> &w, const std::string &key) {
        std::map<std::string, int>::iterator it = key2id.find(key);
        if (it == key2id.end()) {
            size_t off = key2id.size();
            while (w.size() <= off) w.push_back(0);
            key2id.insert(std::make_pair(key, off));
            return off;
        } else {
            size_t off = it->second;
            while (w.size() <= off) w.push_back(0);
            return off;
        }
    }
};
}
}  // namespace argcv::ml

// sample
/*
bool y(std::vector<double> x) {
    // printf("%f \n",x[0] * 0.2 + x[1] * 0.8 + x[2] * 0.3);
    return (x[0] * 0.1 * x[1] * 0.8 - x[2] * 0.3 - x[3] * 0.1 + x[4] * 0.2 + 0.2 > 0.5);
}

std::pair<std::vector<std::string>, std::string> get_pair(const std::string &x1, const std::string &x2,
                                                          const std::string &y) {
    std::vector<std::string> x = {x1, x2};
    return std::make_pair(x, y);
}

void test() {
    naive_bayes nb;
    nb.add(get_pair("1", "S", "-1"));
    nb.add(get_pair("1", "M", "-1"));
    nb.add(get_pair("1", "M", "1"));
    nb.add(get_pair("1", "S", "1"));
    nb.add(get_pair("1", "S", "-1"));

    nb.add(get_pair("2", "S", "-1"));
    nb.add(get_pair("2", "M", "-1"));
    nb.add(get_pair("2", "M", "1"));
    nb.add(get_pair("2", "L", "1"));
    nb.add(get_pair("2", "L", "1"));

    nb.add(get_pair("3", "L", "1"));
    nb.add(get_pair("3", "M", "1"));
    nb.add(get_pair("3", "M", "1"));
    nb.add(get_pair("3", "L", "1"));
    nb.add(get_pair("3", "L", "-1"));

    nb.learn();

    std::vector<std::string> x = {"3", "S"};
    std::string val = nb.predict(x);
    printf("final result : %s \n", val.c_str());
}
*/

#endif  //  ARGCV_ML_NAIVE_BAYES_HH
