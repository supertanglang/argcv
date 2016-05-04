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
#ifndef ARGCV_STRING_CHARS_H
#define ARGCV_STRING_CHARS_H

#ifdef __cplusplus
extern "C" {
#endif

inline int is_western_space(char ch) {
    switch (ch) {
        case 0x09:
        case 0x0A:
        case 0x0D:
        case 0x20:
            return 1;
        default:
            return 0;
    }
}

inline int is_western_punct(char ch) {
    // see if it is either a space, control character, or alphanumeric and negate that
    return ch > 0
           && !(((ch >= 0x30 /*'0'*/) && (ch <= 0x39 /*'9'*/))
                || ((ch >= 0x41 /*'A'*/) && (ch <= 0x5A /*'Z'*/))
                || ((ch >= 0x61 /*'a'*/) && (ch <= 0x7A /*'z'*/)) || ((ch >= 0x00) && (ch <= 0x20)));
}

inline int is_western_hyphen(char ch) { return ch == 0x2D; }

inline int is_western_character(char ch) {
    return (((ch >= 0x30 /*'0'*/) && (ch <= 0x39 /*'9'*/)) || ((ch >= 0x41 /*'A'*/) && (ch <= 0x5A /*'Z'*/))
            || ((ch >= 0x61 /*'a'*/) && (ch <= 0x7A /*'z'*/)));
}

#ifdef __cplusplus
}
#endif

#endif  // ARGCV_STRING_CHARS_H
