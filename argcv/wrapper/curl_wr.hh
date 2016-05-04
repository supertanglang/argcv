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
#include "global/config.h"
#if !OPTIONS_USE_EXTERNAL_CURL
#define ARGCV_WRAPPER_CURL_WR_HH
#endif
#ifndef ARGCV_WRAPPER_CURL_WR_HH
#define ARGCV_WRAPPER_CURL_WR_HH

#include <stdio.h>

#include <openssl/ssl.h>
#include <curl/curl.h>

// http://curl.haxx.se/libcurl/c/curl_easy_setopt.html

#include <string>
#include <vector>

namespace argcv {
namespace wrapper {
namespace curl {

enum class CURL_VERSION : unsigned char { HTTP_VERSION_1_0, HTTP_VERSION_1_1, HTTP_VERSION_2_0 };

class curl_wr_resp {
public:
    std::vector<std::string> headers;
    std::string body;
};


class curl_wr {
public:
    curl_wr(const std::string& url) : url(url), handler(curl_easy_init()) {
        curl_global_init(CURL_GLOBAL_ALL);  // In windows, this will init the winsock stuff
        // handler = curl_easy_init();
        if (handler) {
            curl_easy_setopt(handler, CURLOPT_URL, url.c_str());
            headers = NULL;
            _e = false;
        } else {
            _e = true;
        }
    }

    ~curl_wr() {
        if (!_e) {
            curl_easy_cleanup(handler);
        }
        if (!headers) {
            curl_slist_free_all(headers);
        }
        curl_global_cleanup();
    }

    bool _err() { return _e; }

    void referer(const std::string& rurl);
    void header(const std::string& hstr);
    void useragent(const std::string& useragent);
    void verbose(bool flag = true);

    void conn_timeout(long sec);
    void conn_timeout_ms(long ms);
    void timeout(long sec);
    void timeout_ms(long ms);

    void version(CURL_VERSION v = CURL_VERSION::HTTP_VERSION_1_1);

    curl_wr_resp& get();

    curl_wr_resp& resp() { return _resp; }

private:
    const std::string url;
    CURL* handler;
    struct curl_slist* headers;
    bool _e;
    curl_wr_resp _resp;
    void pre_request();
};
}
}
}  // namespace argcv::wrapper::curl

#endif  // ARGCV_WRAPPER_CURL_WR_HH
