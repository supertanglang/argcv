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
#include "global/config.h" // check OPTIONS_USE_EXTERNAL_CURL
#if OPTIONS_USE_EXTERNAL_CURL

#include "argcv/wrapper/curl_wr.hh"

#include <openssl/ssl.h>
#include <curl/curl.h>

#include <cstdio>

#include <string>

namespace argcv {
namespace wrapper {
namespace curl {

static size_t __curl_header_callback(char *buffer, size_t size, size_t nmemb, void *_resp) {
    size_t rsize = nmemb * size;
    curl_wr_resp *_r = (curl_wr_resp *)_resp;
    _r->headers.push_back(std::string(buffer, rsize));
    return rsize;
}
size_t __curl_write_callback(char *ptr, size_t size, size_t nmemb, void *_resp) {
    size_t rsize = nmemb * size;
    curl_wr_resp *_r = (curl_wr_resp *)_resp;
    _r->body += std::string(ptr, rsize);
    return rsize;
}
void curl_wr::pre_request() {
    // curl_easy_setopt(handler, CURLOPT_NOPROGRESS, 0L);

    curl_easy_setopt(handler, CURLOPT_HTTPHEADER, headers);
    // curl_easy_setopt(handler, CURLOPT_READDATA, &i);

    curl_easy_setopt(handler, CURLOPT_HEADERFUNCTION, __curl_header_callback);
    curl_easy_setopt(handler, CURLOPT_HEADERDATA, &_resp);
    curl_easy_setopt(handler, CURLOPT_WRITEFUNCTION, __curl_write_callback);
    curl_easy_setopt(handler, CURLOPT_WRITEDATA, &_resp);
}

curl_wr_resp &curl_wr::get() {
    if (!_e) {
        pre_request();
        _e = CURLE_OK != curl_easy_perform(handler);
        return _resp;
    } else {
        return _resp;
    }
}

void curl_wr::referer(const std::string &rurl) { curl_easy_setopt(handler, CURLOPT_REFERER, rurl.c_str()); }

void curl_wr::header(const std::string &hstr) { headers = curl_slist_append(headers, hstr.c_str()); }

void curl_wr::useragent(const std::string &useragent) {
    curl_easy_setopt(handler, CURLOPT_USERAGENT, useragent.c_str());
}

void curl_wr::verbose(bool flag) {
    flag ? curl_easy_setopt(handler, CURLOPT_VERBOSE, 1L) : curl_easy_setopt(handler, CURLOPT_VERBOSE, 0L);
}

void curl_wr::conn_timeout(long sec) { curl_easy_setopt(handler, CURLOPT_CONNECTTIMEOUT, sec); }
void curl_wr::conn_timeout_ms(long ms) { curl_easy_setopt(handler, CURLOPT_CONNECTTIMEOUT_MS, ms); }
void curl_wr::timeout(long sec) { curl_easy_setopt(handler, CURLOPT_TIMEOUT, sec); }
void curl_wr::timeout_ms(long ms) { curl_easy_setopt(handler, CURLOPT_TIMEOUT_MS, ms); }

void curl_wr::version(CURL_VERSION v) {
    switch (v) {
        case CURL_VERSION::HTTP_VERSION_1_0:
            curl_easy_setopt(handler, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_0);
            break;
        case CURL_VERSION::HTTP_VERSION_1_1:
            curl_easy_setopt(handler, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
            break;
        case CURL_VERSION::HTTP_VERSION_2_0:
#ifdef CURL_HTTP_VERSION_2_0
            curl_easy_setopt(handler, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_2_0);
#else
            fprintf(stderr, "warning: http/2.0 not supported in current, using http/1.1 instead\n");
            curl_easy_setopt(handler, CURLOPT_HTTP_VERSION, CURL_HTTP_VERSION_1_1);
#endif
            break;
    }
}
}
}
}  // namespace argcv::wrapper::curl

#endif // OPTIONS_USE_EXTERNAL_CURL
