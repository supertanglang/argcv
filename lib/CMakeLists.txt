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
SET(ARGCV_VERSION 1.0)
SET(ARGCV_SOVERSION 0.1)

################### UTIL EXT : GRAPH
AUX_SOURCE_DIRECTORY(. LIBARGCV_SRC)
ADD_LIBRARY(argcv SHARED ${LIBARGCV_SRC})
SET_TARGET_PROPERTIES(argcv PROPERTIES VERSION ${ARGCV_VERSION} SOVERSION ${ARGCV_SOVERSION})

ADD_LIBRARY(argcv_static STATIC ${LIBARGCV_SRC})
SET_TARGET_PROPERTIES(argcv_static PROPERTIES OUTPUT_NAME "argcv")

#ADD_DEPENDENCIES(argcv leveldb)
#ADD_DEPENDENCIES(argcv crfpp)
#ADD_DEPENDENCIES(argcv mongo_cxx_static)

#ADD_DEPENDENCIES(argcv_static leveldb_static)
#ADD_DEPENDENCIES(argcv_static crfpp_static)
#ADD_DEPENDENCIES(argcv_static mongo_cxx_static)

#TARGET_LINK_LIBRARIES(argcv leveldb)
#TARGET_LINK_LIBRARIES(argcv crfpp)
#TARGET_LINK_LIBRARIES(argcv mongo_cxx_static)
#TARGET_LINK_LIBRARIES(argcv pthread)

#TARGET_LINK_LIBRARIES(argcv_static leveldb_static)
#TARGET_LINK_LIBRARIES(argcv_static crfpp_static)
#TARGET_LINK_LIBRARIES(argcv_static mongo_cxx_static)
#TARGET_LINK_LIBRARIES(argcv_static pthread)

# INSTALL (TARGETS argcv_util argcv_util_static
#     LIBRARY DESTINATION lib64
#     ARCHIVE DESTINATION lib64)

# INSTALL (FILES ${${CMAKE_PROJECT_NAME}_SOURCE_DIR}/include/argcv/util.h DESTINATION include/argcv/ )

ADD_SUBDIRECTORY(graph)
ADD_SUBDIRECTORY(ir)
ADD_SUBDIRECTORY(storage)
ADD_SUBDIRECTORY(string)
ADD_SUBDIRECTORY(sys)
ADD_SUBDIRECTORY(wrapper)

