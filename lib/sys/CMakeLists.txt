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
SET(ARGCV_SYS_VERSION 1.0)
SET(ARGCV_SYS_SOVERSION 0.1)

ADD_DEFINITIONS( " -D_POSIX_SOURCE ")

AUX_SOURCE_DIRECTORY(. LIBARGCV_SYS_SRC)
ADD_LIBRARY(argcv_sys SHARED ${LIBARGCV_SYS_SRC})
SET_TARGET_PROPERTIES(argcv_sys PROPERTIES VERSION ${ARGCV_SYS_VERSION} SOVERSION ${ARGCV_SYS_SOVERSION})

ADD_LIBRARY(argcv_sys_static STATIC ${LIBARGCV_SYS_SRC})
SET_TARGET_PROPERTIES(argcv_sys_static PROPERTIES OUTPUT_NAME "argcv_sys")

#ADD_DEPENDENCIES(argcv_sys leveldb_proj)
#TARGET_LINK_LIBRARIES(argcv_sys leveldb)

#ADD_DEPENDENCIES(argcv_sys_static leveldb_proj)

#TARGET_LINK_LIBRARIES(argcv_sys_static argcv_util_static)

# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )



