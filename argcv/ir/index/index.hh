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
#ifndef ARGCV_IR_INDEX_INDEX_HH
#define ARGCV_IR_INDEX_INDEX_HH

#include <cmath>

#include <algorithm>  // std::sort
#include <string>
#include <mutex>  // std::mutex

#include "doc.hh"
#include "field.hh"
#include "util.hh"
#include "analyzer/analyzer.hh"
#include "argcv/storage/storage.hh"
#include "argcv/string/string.hh"
#include "argcv/string/uuid.hh"
//#include "argcv/wrapper/leveldb_wr.hh"

namespace argcv {
namespace ir {
namespace index {

// using ::argcv::wrapper::leveldb::ldb_wr;
using ::argcv::storage::storage;
using ::argcv::storage::stg_bw_handler;
using ::argcv::string::as_type;
using ::argcv::string::as_str;

class index {
public:
    index(storage* _stg);
    virtual ~index();

    bool db_status() { return !_stg->is_closed(); }

    /**
     * get a document by id
     */
    std::string get(const std::string& id) {
        std::string k = DB_DOC_PREFIX;
        k += id;
        std::string val;
        _stg->get(k, &val);
        return val;
    }

    /**
     * save a doc by id
     */
    bool save(const std::string& id, const std::string& val) {
        std::string k = DB_DOC_PREFIX;
        k += id;
        return _stg->put(k, val);
    }

    /**
     * get stored key set in index
     */
    std::vector<std::string> get_keys(const std::string& id, const std::string& field_name,
                                      std::vector<std::string>* _keys) {
        std::string val;
        _stg->get(idx_meta_keyset(id, field_name), &val);
        size_t sz_val = val.size();
        size_t offset = 0;
        size_t sz_sz = sizeof(size_t);
        _keys->clear();
        while (offset < sz_val) {
            size_t sz = as_type<size_t>(val, offset);
            offset += sz_sz;
            _keys->push_back(val.substr(offset, sz));
            offset += sz;
        }
        return *_keys;
    }

    size_t _sz_cur_doc(const std::string& id, const std::string& field_name) {
        std::string val;
        _stg->get(idx_meta_sz_cur_doc(id, field_name), &val);
        return as_type<size_t>(val);
    }

    size_t _sz_all_doc(const std::string& field_name) {
        std::string val;
        _stg->get(idx_meta_sz_all_doc(field_name), &val);
        return as_type<size_t>(val);
    }

    size_t _sz_all_doc_incr(const std::string& field_name, bool with_lock = true) {
        if (with_lock) {
            _lock_w->lock();
        }
        std::string val;
        _stg->get(idx_meta_sz_all_doc(field_name), &val);
        size_t sz = as_type<size_t>(val) + 1;
        _stg->put(idx_meta_sz_all_doc(field_name), as_str<size_t>(sz));
        if (with_lock) {
            _lock_w->unlock();
        }
        return sz;
    }

    /**
     * save stored keyset in index(base on id and field_name)
     */
    bool save_keys(const std::string& id, const std::string& field_name,
                   const std::vector<std::string>& keys) {
        return _stg->put(idx_meta_keyset(id, field_name), keyset_meta_gen(id, field_name, keys));
    }

    int64_t rm_keys(const std::string& id, const std::string& field_name, bool with_lock = true) {
        if (with_lock) {
            _lock_w->lock();
            // printf("writer:: lock\n");
            // fflush(NULL);
        }

        std::vector<std::string> keys;
        get_keys(id, field_name, &keys);
        auto bw = _stg->batch_writer();
        const std::string prefix(idx_key_prefix(field_name));
        for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); it++) {
            std::string key(prefix);
            key += *it;
            // printf("rm : %zu %s\n", key.length(), key.c_str());
            bw->rm(key);
        }
        // printf("rm : %zu %s\n", idx_meta_keyset(id, field_name).length(),
        //       idx_meta_keyset(id, field_name).c_str());
        bw->rm(idx_meta_keyset(id, field_name));
        // printf("rm : %zu %s\n", idx_meta_sz_cur_doc(id, field_name).length(),
        //       idx_meta_sz_cur_doc(id, field_name).c_str());
        bw->rm(idx_meta_sz_cur_doc(id, field_name));
        bool status = bw->commit();
        delete bw;
        if (with_lock) {
            _lock_w->unlock();
            // printf("writer:: unlock\n");
            // fflush(NULL);
        }

        return status ? (int64_t)(keys.size()) : -1;
    }

    /**
     * save a field
     */
    template <typename T>
    bool save_field(const std::string& id, const std::string& field_name, field<T> field_value) {
        _lock_w->lock();
        // printf("writer:: lock\n");
        // fflush(NULL);
        int64_t sz_rm = rm_keys(id, field_name, false);  // clear without lock, because of it had been locked
        if (sz_rm == 0) {
            _sz_all_doc_incr(field_name, false);
        }
        std::vector<std::pair<std::string, std::string>> key_values = field_value.key_values(id);
        auto bw = _stg->batch_writer();

        // save term=>positions
        const std::string prefix(idx_key_prefix(field_name));
        for (std::vector<std::pair<std::string, std::string>>::const_iterator it = key_values.begin();
             it != key_values.end(); it++) {
            std::string _k(prefix);
            _k += it->first;
            bw->put(_k, it->second);
        }
        // save all doc size
        bw->put(idx_meta_sz_cur_doc(id, field_name), as_str<size_t>(field_value.size()));
        // save meta of keyset
        bw->put(idx_meta_keyset(id, field_name), keyset_meta_gen(id, field_name, field_value.keys(id)));

        bool status = bw->commit();
        delete bw;
        _lock_w->unlock();
        // printf("commit %s %d: %s\n", __FILE__, __LINE__, status ? "ok" : "failed");
        // printf("writer:: unlock\n");
        // fflush(NULL);
        return status;
    }

    std::vector<std::pair<std::string, double>> search(const std::string& field_name,
                                                       std::vector<std::string> terms, size_t limit = 0,
                                                       double delta = 0.01) {
        std::vector<std::pair<std::string, double>> ids;
        const std::string prefix(idx_key_prefix(field_name));
        std::map<std::string, std::pair<double, size_t>> sr_cnter;
        // std::pair<std::vector<doc>, size_t> docs = std::make_pair(std::vector<doc>(), prefix.length());
        docs dc;
        dc.sz_prefix = prefix.length();
        dc.sz_limit = limit;
        size_t ads = _sz_all_doc(field_name);  // ads : all document size
        for (size_t ix = 0; ix < terms.size(); ix++) {
            dc.data.clear();
            std::string skey(prefix);
            skey += terms[ix];
            skey += DB_SEPARATOR;
            _stg->start_with(prefix + terms[ix], _term_gather, &dc);
            size_t dscct = dc.sz_data;
            for (size_t ix = 0; ix < dc.data.size(); ix++) {
                doc d = dc.data[ix];
                size_t stid = d.size();
                size_t atsid = _sz_cur_doc(d.id(), field_name);
                double score = tf_idf(stid, atsid, ads, dscct);
                if (sr_cnter.find(d.id()) == sr_cnter.end()) {
                    sr_cnter[d.id()] = std::make_pair(score, 1);
                } else {
                    sr_cnter[d.id()] = std::make_pair(sr_cnter[d.id()].first * score,  //
                                                      sr_cnter[d.id()].second + 1);
                }
            }
        }
        double w = 1;  // pow(1.0 / delta, terms.size());
        for (std::map<std::string, std::pair<double, size_t>>::iterator it = sr_cnter.begin();
             it != sr_cnter.end(); it++) {
            std::pair<double, size_t> cnt = it->second;
            if (cnt.second < terms.size()) {
                cnt.first *= pow(delta, terms.size() - cnt.second);
            }
            // cnt.first *= pow(delta, terms.size() - cnt.second);
            if (cnt.first > 0) {
                ids.push_back(std::make_pair(it->first, cnt.first * w));
            }
        }
        std::sort(ids.begin(), ids.end(), tfidf_score_sort);
        return ids;
    }

    void dump() {
        int i = 0;
        _stg->start_with("", _dump, &i);
    }

private:
    storage* _stg;
    std::mutex* _lock_w;  // write locker
    const std::string idx_key_prefix(const std::string& field_name) {
        std::string _prefix(DB_IDX_PREFIX);
        _prefix += DB_SEPARATOR;
        _prefix += field_name;
        _prefix += DB_SEPARATOR;
        return _prefix;
    }
    const std::string idx_meta_keyset(const std::string& id, const std::string& field_name) {
        std::string _prefix(DB_META_PREFIX);
        _prefix += DB_SEPARATOR;
        _prefix += "KEYS";  //
        _prefix += DB_SEPARATOR;
        _prefix += field_name;
        _prefix += DB_SEPARATOR;
        _prefix += id;
        return _prefix;
    }
    const std::string idx_meta_sz_cur_doc(const std::string& id, const std::string& field_name) {
        std::string _prefix(DB_META_PREFIX);
        _prefix += DB_SEPARATOR;
        _prefix += "SZ_CUR_DOC";  //
        _prefix += DB_SEPARATOR;
        _prefix += field_name;
        _prefix += DB_SEPARATOR;
        _prefix += id;
        return _prefix;
    }

    const std::string idx_meta_sz_all_doc(const std::string& field_name) {
        std::string _prefix(DB_META_PREFIX);
        _prefix += DB_SEPARATOR;
        _prefix += "SZ_ALL_DOC";
        return _prefix;
    }

    std::string keyset_meta_gen(const std::string& id, const std::string& field_name,
                                const std::vector<std::string>& keys) {
        std::string val;
        for (std::vector<std::string>::const_iterator it = keys.begin(); it != keys.end(); it++) {
            val += as_str<size_t>(it->size());
            val += *it;
        }
        return val;
    }

    static bool _dump(const std::string& k, const std::string& v, void* data) {
        int* _offset = (int*)data;
        printf("%d key: %s \t value: %s \n", (*_offset)++, k.c_str(), v.c_str());
        return true;  //(*_offset) < 2;
    }

    static bool _term_gather(const std::string& k, const std::string& v, void* data) {
        // std::pair<std::vector<doc>, size_t>* docs = (std::pair<std::vector<doc>, size_t>*)data;
        docs* _dc = (docs*)data;

        if (_dc->sz_limit == 0 || _dc->sz_limit >= _dc->data.size()) {
            doc d(k, v, _dc->sz_prefix);
            /*
            printf("id : %s term: %s size: %zu\n", d.id_s().c_str(), d.term().c_str(), d.size());
            std::vector<size_t> vpos = d.vpos();
            for (size_t i = 0; i < vpos.size(); i++) {
                printf("pos: %zu \n", vpos[i]);
            }*/
            _dc->data.push_back(d);
        }
        _dc->sz_data++;
        return true;  //(*_offset) < 2;
    }

    // tf-idf k in document D
    // stid : size of term k in  D
    // atsid :  all term size in D
    // ads : all document size
    // dscct : document size contains current term
    inline double tf_idf(size_t stid, size_t atsid, size_t ads, size_t dscct) {
        //#define MATH_LG_10 2.302585
        // tf * idf
        if (ads == 0 || atsid == 0 || dscct == 0) return 0;
        return ((double)stid / atsid) * log((double)ads / (dscct)) / 2.302585;
    }

    static bool tfidf_score_sort(std::pair<std::string, double> lf, std::pair<std::string, double> ri) {
        return (lf.second > ri.second);
    }
};

// sometimes, we may find a warning as follow :
// xx.cc:123:4: error: must use 'class' tag to refer to type 'index' in this scope
// /usr/include/strings.h:73:7: note: class 'index' is hidden by a non-type declaration of 'index' here
// char    *index(const char *, int) __POSIX_C_DEPRECATED(200112L);
// well, another alias may helps.
typedef ::argcv::ir::index::index aidx;
}
}
}  // namespace argcv::ir::index

#endif  //  ARGCV_IR_INDEX_INDEX_HH
