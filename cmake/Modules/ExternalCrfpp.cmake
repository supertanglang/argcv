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
SET(crfpp_build "${CMAKE_CURRENT_BINARY_DIR}/crfpp")

INCLUDE(ProcessorCount)
ProcessorCount(PROCESSOR_COUNT_VAL)


EXTERNALPROJECT_ADD(
    crfpp_proj
    GIT_REPOSITORY https://github.com/yuikns/crfpp.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND ./configure
    SOURCE_DIR ${crfpp_build}
    # BINARY_DIR ${crfpp_build}
    BUILD_COMMAND make -j${PROCESSOR_COUNT_VAL}
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
)


SET(CRFPP_INCLUDE_DIR ${crfpp_build})
SET(CRFPP_LIB_DIR ${crfpp_build}/.libs)

ADD_LIBRARY(crfpp SHARED IMPORTED)
SET_PROPERTY(TARGET crfpp PROPERTY IMPORTED_LOCATION ${crfpp_build}/.libs/${CMAKE_SHARED_LIBRARY_PREFIX}crfpp${CMAKE_SHARED_LIBRARY_SUFFIX})

MESSAGE(STATUS "ARGCV CRFPP SHARD " ${crfpp_build}/${CMAKE_SHARED_LIBRARY_PREFIX}crfpp${CMAKE_SHARED_LIBRARY_SUFFIX})

ADD_LIBRARY(crfpp_static STATIC IMPORTED)
SET_PROPERTY(TARGET crfpp_static PROPERTY IMPORTED_LOCATION ${crfpp_build}/.libs/${CMAKE_STATIC_LIBRARY_PREFIX}crfpp${CMAKE_STATIC_LIBRARY_SUFFIX})

MESSAGE(STATUS "ARGCV CRFPP STATIC " ${crfpp_build}/${CMAKE_STATIC_LIBRARY_PREFIX}crfpp${CMAKE_STATIC_LIBRARY_SUFFIX})

ADD_DEPENDENCIES(crfpp crfpp_proj)

ADD_DEPENDENCIES(crfpp_static crfpp_proj)

SET(CRFPP_LIBRARY ${crfpp})
SET(CRFPP_LIBRARY_STATIC ${crfpp_static})



