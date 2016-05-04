# The MIT License (MIT)
#
# Copyright (c) 2015 Yu Jing <yu@argcv.com>
#
# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files (the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions:
#
# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.
#
# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
#
SET(ARGCV_WRAPPER_INI_VERSION 1.0)
SET(ARGCV_WRAPPER_INI_SOVERSION 0.1)

AUX_SOURCE_DIRECTORY(. LIBARGCV_WRAPPER_INI_SRC)
ADD_LIBRARY(argcv_wrapper_ini SHARED ${LIBARGCV_WRAPPER_INI_SRC})
SET_TARGET_PROPERTIES(argcv_wrapper_ini PROPERTIES VERSION ${ARGCV_WRAPPER_INI_VERSION} SOVERSION ${ARGCV_WRAPPER_INI_SOVERSION})

ADD_LIBRARY(argcv_wrapper_ini_static STATIC ${LIBARGCV_WRAPPER_INI_SRC})
SET_TARGET_PROPERTIES(argcv_wrapper_ini_static PROPERTIES OUTPUT_NAME "argcv_wrapper_ini")





