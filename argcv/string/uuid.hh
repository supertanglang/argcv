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
#ifndef ARGCV_STRING_UUID_HH
#define ARGCV_STRING_UUID_HH

#include <ctime>
#include <cstring>  //  memset

#include <pthread.h>
#include <sys/time.h>
#ifdef __MACH__
#include <mach/clock.h>
#include <mach/mach.h>
#endif

#include <string>
#include <utility>

#include "argcv/random/random.hh"
#include "argcv/string/string.hh"

namespace argcv {
namespace string {

// rel: https://tools.ietf.org/html/rfc4122
// rel: http://docs.oracle.com/javase/7/docs/api/java/util/UUID.html
// rel: https://support.datastax.com/hc/en-us/articles/204226019-Converting-TimeUUID-Strings-to-Dates
class uuid {
public:
    uuid(uint16_t node = 0) { assign(node); }
    uuid(uint64_t _hi, uint64_t _lo) : _hi(_hi), _lo(_lo) {}
    uuid(const std::string& str) {
        _hi = 0;
        _lo = 0;
        if (str.size() == 36 && str[8] == '-' && str[13] == '-' && str[18] == '-' && str[23] == '-') {
            // string
            for (size_t i = 0; i < 18; i++) {
                if (str[i] >= 'a' && str[i] <= 'f') {
                    _hi = _hi << 4 | (str[i] - 'a' + 10);
                } else if (str[i] >= 'A' && str[i] <= 'Z') {
                    _hi = _hi << 4 | (str[i] - 'A' + 10);
                } else if (str[i] >= '0' && str[i] <= '9') {
                    _hi = _hi << 4 | (str[i] - '0');
                }
            }
            for (size_t i = 18; i < 36; i++) {
                if (str[i] >= 'a' && str[i] <= 'f') {
                    _lo = _lo << 4 | (str[i] - 'a' + 10);
                } else if (str[i] >= 'A' && str[i] <= 'Z') {
                    _lo = _lo << 4 | (str[i] - 'A' + 10);
                } else if (str[i] >= '0' && str[i] <= '9') {
                    _lo = _lo << 4 | (str[i] - '0');
                }
            }
        } else if (str.size() == 32) {
            // hex
            for (size_t i = 0; i < 16; i++) {
                if (str[i] >= 'a' && str[i] <= 'f') {
                    _hi = _hi << 4 | (str[i] - 'a' + 10);
                } else if (str[i] >= 'A' && str[i] <= 'Z') {
                    _hi = _hi << 4 | (str[i] - 'A' + 10);
                } else if (str[i] >= '0' && str[i] <= '9') {
                    _hi = _hi << 4 | (str[i] - '0');
                }
            }
            for (size_t i = 16; i < 32; i++) {
                if (str[i] >= 'a' && str[i] <= 'f') {
                    _lo = _lo << 4 | (str[i] - 'a' + 10);
                } else if (str[i] >= 'A' && str[i] <= 'Z') {
                    _lo = _lo << 4 | (str[i] - 'A' + 10);
                } else if (str[i] >= '0' && str[i] <= '9') {
                    _lo = _lo << 4 | (str[i] - '0');
                }
            }
        } else if (str.size() == sizeof(uint64_t) * 2) {
            // binary
            size_t sz = sizeof(uint64_t);
            memcpy(&_hi, str.data(), sz);
            memcpy(&_lo, str.data() + sz, sz);
        }
    }

    bool assign(uint16_t node = 0) {
        uint64_t seed = _uuid_ts();

        uint32_t time_low = seed & 0xFFFFFFFF;
        uint16_t time_mid = (seed >> 32) & 0xFFFF;
        uint16_t time_hi_version = (seed >> 48) & 0xFFF;

        memset(&_hi, 0x0, sizeof(_hi));
        memset(&_lo, 0x0, sizeof(_lo));

        // version = 1 , ref: rfc4122
        _hi = (uint64_t)time_low << 32 | (uint64_t)time_mid << 16 | (uint64_t)time_hi_version;

        // set version
        _hi &= ~0xF000;
        _hi |= _version << 12;

        // _lo
        uint16_t clock_seq = argcv::random::random_int() & 0x3FFF;  //
        // uint8_t clock_seq_low = clock_seq & 0xFF;
        // uint8_t clock_seq_hi_variant = (clock_seq >> 8) & 0x3F;

        _lo = (uint64_t)clock_seq << 48 | node;

        // variant = 2  ref: rfc4122
        _lo &= ~((uint64_t)0xC000 << 48);
        _lo |= (uint64_t)0x8000 << 48;

        return true;
    }

    ~uuid() {}

    std::string hex() {
        const size_t sz_buff = sizeof(_hi) * 4;  //
        char buff[sz_buff];                      //
        sprintf(buff, "%016llx%016llx", (long long unsigned int)_hi, (long long unsigned int)_lo);
        return std::string(buff, sz_buff);
    }

    std::string data() { return as_str<uint64_t>(_hi) += as_str<uint64_t>(_lo); }

    std::string str() {
        std::string s_val;
        std::string s_hex = hex();
        for (int i = 0; i < 8; i++) {
            s_val += s_hex[i];
        }
        s_val += '-';
        for (int i = 8; i < 12; i++) {
            s_val += s_hex[i];
        }
        s_val += '-';
        for (int i = 12; i < 16; i++) {
            s_val += s_hex[i];
        }
        s_val += '-';
        for (int i = 16; i < 20; i++) {
            s_val += s_hex[i];
        }
        s_val += '-';
        for (int i = 20; i < 32; i++) {
            s_val += s_hex[i];
        }
        return s_val;
    }

    std::pair<uint64_t, uint64_t> pair() { return std::make_pair(_hi, _lo); }

    bool operator==(uuid const& rhs) { return _lo == rhs._lo && _hi == rhs._hi; }

    bool operator!=(uuid const& rhs) { return !(*this == rhs); }

    bool operator>(uuid const& rhs) { return _hi > rhs._hi; }

    bool operator<(uuid const& rhs) { return _hi < rhs._hi; }

private:
    uint64_t _hi;
    uint64_t _lo;

    static const uint64_t _version = 1;  //  uuid version

    inline uint64_t _uuid_ts() {
        static uint64_t lts = 0;
        struct timespec ts;
#ifdef __MACH__
        // OS X does not have clock_gettime, use clock_get_time
        clock_serv_t cclock;
        mach_timespec_t mts;
        host_get_clock_service(mach_host_self(), CALENDAR_CLOCK, &cclock);
        clock_get_time(cclock, &mts);
        mach_port_deallocate(mach_task_self(), cclock);
        ts.tv_sec = mts.tv_sec;
        ts.tv_nsec = mts.tv_nsec;
#else
        clock_gettime(CLOCK_REALTIME, &ts);
#endif
        uint64_t cts = ts.tv_sec * 10000000 + (ts.tv_nsec / 100) + 0x01b21dd213814000;
        if (cts <= lts) {
            cts = ++lts;
        } else {
            lts = cts;
        }
        return cts;
    }
};
}
}  // argcv::string

//  usage :
/*
using namespace argcv::string;

int main(int argc, char * argv[]) {
    uuid u1(0x33);
    uuid u2(0x33);
    for(int i = 0 ; i < 8; i ++ ) {
        printf("%c",u1.hex()[i]);
    }
    printf("-");
    for(int i = 8 ; i < 12; i ++ ) {
        printf("%c",u1.hex()[i]);
    }
    printf("-");
    for(int i = 12; i < 16; i ++ ) {
        printf("%c",u1.hex()[i]);
    }
    printf("-");
    for(int i = 16; i < 20; i ++ ) {
        printf("%c",u1.hex()[i]);
    }
    printf("-");
    for(int i = 20; i < 32; i ++ ) {
        printf("%c",u1.hex()[i]);
    }
    printf("\n");
    std::pair<uint64_t,uint64_t> p = u1.pair();
    printf("%s # %llx %llx\n",u1.hex().c_str(),p.first,p.second);

    p = u2.pair();
    printf("%s # %llx %llx\n",u2.hex().c_str(),p.first,p.second);

    return 0;
}
*/

#endif  //  ARGCV_STRING_UUID_HH