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
SET(ARGCV_IR_INDEX_VERSION 1.0)
SET(ARGCV_IR_INDEX_SOVERSION 0.1)

AUX_SOURCE_DIRECTORY(. LIBARGCV_IR_INDEX_SRC)
ADD_LIBRARY(argcv_ir_index SHARED ${LIBARGCV_IR_INDEX_SRC})
SET_TARGET_PROPERTIES(argcv_ir_index PROPERTIES VERSION ${ARGCV_IR_INDEX_VERSION} SOVERSION ${ARGCV_IR_INDEX_SOVERSION})

ADD_LIBRARY(argcv_ir_index_static STATIC ${LIBARGCV_IR_INDEX_SRC})
SET_TARGET_PROPERTIES(argcv_ir_index_static PROPERTIES OUTPUT_NAME "argcv_ir_index")


TARGET_LINK_LIBRARIES(argcv_ir_index argcv_wrapper_leveldb)
TARGET_LINK_LIBRARIES(argcv_ir_index pthread) # for mutex
TARGET_LINK_LIBRARIES(argcv_ir_index m) # for math

TARGET_LINK_LIBRARIES(argcv_ir_index_static argcv_wrapper_leveldb_static)
TARGET_LINK_LIBRARIES(argcv_ir_index_static pthread) # for mutex
TARGET_LINK_LIBRARIES(argcv_ir_index m) # for math

