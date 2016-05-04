#include "argcv/wrapper/svm_light_wr.hh"

#include <cstdint>  // uint64_t
#include <cstdio>

#include <map>
#include <string>
#include <utility>  // std::pair, std::make_pair
#include <vector>

#include "argcv/ml/ml.hh"

/* if svm-learn is used out of C++, define it as extern "C" */
#ifdef __cplusplus
extern "C" {
#endif

#include "svm_common.h"
#include "svm_learn.h"

#ifdef __cplusplus
}
#endif

namespace argcv {
namespace wrapper {
namespace svm_light_wr {

using namespace ::argcv::ml;

class svm_light_wr_data {
public:
    LEARN_PARM learn_parm;
    KERNEL_PARM kernel_parm;
    size_t totdoc;
    size_t totwords;
    MODEL *_model;
    DOC **_docs;
    double *_target;
    double *_alpha_in;

    svm_light_wr_data()
        : totdoc(0), totwords(0), _model(nullptr), _docs(nullptr), _target(nullptr), _alpha_in(nullptr) {
        LEARN_PARM *_learn_parm = &learn_parm;
        KERNEL_PARM *_kernel_parm = &kernel_parm;
        strcpy(_learn_parm->predfile, "trans_predictions");
        strcpy(_learn_parm->alphafile, "");
        _learn_parm->biased_hyperplane = 1;
        _learn_parm->sharedslack = 0;
        _learn_parm->remove_inconsistent = 0;
        _learn_parm->skip_final_opt_check = 0;
        _learn_parm->svm_maxqpsize = 10;
        _learn_parm->svm_newvarsinqp = 0;
        _learn_parm->svm_iter_to_shrink = -9999;
        _learn_parm->maxiter = 100000;
        _learn_parm->kernel_cache_size = 40;
        _learn_parm->svm_c = 10000; //0.00
        _learn_parm->eps = 0.1;
        _learn_parm->transduction_posratio = -1.0;
        _learn_parm->svm_costratio = 1.0;
        _learn_parm->svm_costratio_unlab = 1.0;
        _learn_parm->svm_unlabbound = 1E-5;
        _learn_parm->epsilon_crit = 0.001;
        _learn_parm->epsilon_a = 1E-15;
        _learn_parm->compute_loo = 0;
        _learn_parm->rho = 1.0;
        _learn_parm->xa_depth = 0;
        _learn_parm->type = CLASSIFICATION;

        _kernel_parm->kernel_type = 0;
        _kernel_parm->poly_degree = 3;
        _kernel_parm->rbf_gamma = 1.0;
        _kernel_parm->coef_lin = 1;
        _kernel_parm->coef_const = 1;
        strcpy(_kernel_parm->custom, "empty");
    }

    bool load_docs(dataset<double, double> data) {
        char comments[1] = "";
        free_docs();
        _docs = (DOC **)my_malloc(sizeof(DOC *) * data.size());
        _target = (double *)my_malloc(sizeof(double) * data.size());
        totdoc = data.size();
        totwords = data.x_size();
        printf("totdoc: %zu , totwords %zu \n", totdoc, totwords);
        size_t sz_word = sizeof(WORD) * (data.x_size() + 10);
        printf("wsize: %lu\n", sz_word);
        fflush(nullptr);
        WORD *_words = (WORD *) malloc(sz_word);
        for (int i = 0; i < data.size(); i++) {
            for (int j = 0; j < data.x_size(); j++) {
                _words[j].wnum = j + 1;
                _words[j].weight = (FVAL)data.x_at((uint64_t) i, (uint64_t) j);
            }
            _words[data.x_size()].wnum = 0;
            _docs[i] = create_example(i, 0, 0, 1.0, create_svector(_words, comments, 1.0));
            _target[i] = data.y_at(i);
        }
        free(_words);
        return true;
    }

    bool opt(const std::string &key, const std::string &value) { return false; }
    bool opt(const std::string &key, uint64_t value) { return false; }
    bool opt(const std::string &key, double value) { return false; }
    bool opt(const std::string &key, bool value) { return false; }

    ~svm_light_wr_data() {
        free_docs();
        free_model();
        free_alpha_in();
    }

    void free_model() {
        if (_model != nullptr) {
            ::free_model(_model, 0);
            _model = nullptr;
        }
    }

    void free_docs() {
        //totdoc = 0;
        //totwords = 0;
        if (_docs != nullptr) {
            for (int i = 0; i < totdoc; i++) free_example(_docs[i], 1);
            free(_docs);
            _docs = nullptr;
        }
        if (_target != nullptr) {
            free(_target);
            _target = nullptr;
        }
    }

    void free_alpha_in() {
        if (_alpha_in != nullptr) {
            free(_alpha_in);
            _alpha_in = nullptr;
        }
    }
};

svm_light_wr::svm_light_wr() : _sl_info(nullptr) { init(dataset<double, double>()); }

svm_light_wr::svm_light_wr(dataset<double, double> d) : _sl_info(nullptr) { init(d); }

svm_light_wr::~svm_light_wr() { free_all(); }

bool svm_light_wr::learn() {
    _sl_info->load_docs(data);
    KERNEL_CACHE *_kernel_cache =
        _sl_info->kernel_parm.kernel_type == LINEAR
            ? nullptr
            : kernel_cache_init(_sl_info->totdoc, _sl_info->learn_parm.kernel_cache_size);

    _sl_info->free_model();

    //printf("size of model: %zu\n", sizeof(MODEL));
    //fflush(nullptr);
    _sl_info->_model = (MODEL *)my_malloc(sizeof(MODEL));

    //printf("allocated ..\n");
    //fflush(nullptr);

    KERNEL_CACHE **__kernel_cache = _kernel_cache == nullptr ? nullptr : &_kernel_cache;

    //printf("[check point] %s:%d..\n", __FILE__, __LINE__);
    //fflush(nullptr);

    if (_sl_info->learn_parm.type == CLASSIFICATION) {
        svm_learn_classification(_sl_info->_docs, _sl_info->_target, _sl_info->totdoc, _sl_info->totwords,
                                 &(_sl_info->learn_parm), &(_sl_info->kernel_parm), _kernel_cache,
                                 _sl_info->_model, _sl_info->_alpha_in);
    } else if (_sl_info->learn_parm.type == REGRESSION) {
        svm_learn_regression(_sl_info->_docs, _sl_info->_target, _sl_info->totdoc, _sl_info->totwords,
                             &(_sl_info->learn_parm), &(_sl_info->kernel_parm), __kernel_cache,
                             _sl_info->_model);
    } else if (_sl_info->learn_parm.type == RANKING) {
        svm_learn_ranking(_sl_info->_docs, _sl_info->_target, _sl_info->totdoc, _sl_info->totwords,
                          &(_sl_info->learn_parm), &(_sl_info->kernel_parm), __kernel_cache,
                          _sl_info->_model);
    } else if (_sl_info->learn_parm.type == OPTIMIZATION) {
        svm_learn_optimization(_sl_info->_docs, _sl_info->_target, _sl_info->totdoc, _sl_info->totwords,
                               &(_sl_info->learn_parm), &(_sl_info->kernel_parm), _kernel_cache,
                               _sl_info->_model, _sl_info->_alpha_in);
    }
    
    //printf("[check point] %s:%d..\n", __FILE__, __LINE__);
    
    if (_kernel_cache != nullptr) {
        /* Free the memory used for the cache. */
        kernel_cache_cleanup(_kernel_cache);
    }
    //printf("[check point] %s:%d..\n", __FILE__, __LINE__);
    //_sl_info->free_docs();
    
    if (_sl_info->_model->kernel_parm.kernel_type == LINEAR) { /* linear kernel */
        /* compute weight vector */
        add_weight_vector_to_linear_model(_sl_info->_model);
    }
    return true;
}

bool svm_light_wr::save(const std::string &path) {
    write_model((char *)path.c_str(), copy_model(_sl_info->_model));
    return true;
}

bool svm_light_wr::load(const std::string &path) {
    _sl_info->free_model();
    _sl_info->_model = read_model((char *)path.c_str());
    if (_sl_info->_model->kernel_parm.kernel_type == LINEAR) { /* linear kernel */
        /* compute weight vector */
        add_weight_vector_to_linear_model(_sl_info->_model);
    }
    return false;
}

double svm_light_wr::predict(std::vector<double> x) {
    DOC *doc; /* test example */
    WORD *_words = (WORD *)my_malloc(sizeof(WORD) * (x.size() + 10));
    char comments[1] = "";
    double dist;
    for (int i = 0; i < x.size(); i++) {
        (_words[i]).wnum = i + 1;
        (_words[i]).weight = (FVAL)x[i];
    }
    if (_sl_info->_model->kernel_parm.kernel_type == LINEAR) { /* linear kernel */
        for (int j = 0; (_words[j]).wnum != 0; j++) {          /* Check if feature numbers   */
            if ((_words[j]).wnum > _sl_info->_model->totwords) /* are not larger than in     */
                (_words[j]).wnum = 0;                          /* model. Remove feature if   */
        }                                                      /* necessary.                 */
        doc = create_example(-1, 0, 0, 0.0, create_svector(_words, comments, 1.0));
        dist = classify_example_linear(_sl_info->_model, doc);
        free_example(doc, 1);

    } else {
        doc = create_example(-1, 0, 0, 0.0, create_svector(_words, comments, 1.0));
        dist = classify_example(_sl_info->_model, doc);
        free_example(doc, 1);
    }
    free(_words);
    return dist;
}

bool svm_light_wr::opt(const std::string &key, const std::string &value) { return _sl_info->opt(key, value); }
bool svm_light_wr::opt(const std::string &key, uint64_t value) { return _sl_info->opt(key, value); }
bool svm_light_wr::opt(const std::string &key, double value) { return _sl_info->opt(key, value); }
bool svm_light_wr::opt(const std::string &key, bool value) { return _sl_info->opt(key, value); }

void svm_light_wr::init(dataset<double, double> d) {
    free_all();
    verbosity = 1;
    _sl_info = new svm_light_wr_data;
    data = d;
}

void svm_light_wr::free_all() {
    if (_sl_info != nullptr) {
        delete _sl_info;
        _sl_info = nullptr;
    }
}
}
}
}  // namespace argcv::wrapper::svm_light_wr
