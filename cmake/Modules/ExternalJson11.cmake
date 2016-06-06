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
SET(json11_build "${CMAKE_CURRENT_BINARY_DIR}/json11")

EXTERNALPROJECT_ADD(
    json11_proj
    GIT_REPOSITORY https://github.com/dropbox/json11.git
    GIT_TAG master
    PREFIX ${CMAKE_CURRENT_BINARY_DIR}
    UPDATE_COMMAND ""
    CONFIGURE_COMMAND cmake .
    SOURCE_DIR ${json11_build}
    # BINARY_DIR ${json11_build}
    BUILD_COMMAND make json11
    BUILD_IN_SOURCE 1
    INSTALL_COMMAND ""
)


SET(JSON11_INCLUDE_DIR ${json11_build})
SET(JSON11_LIB_DIR ${json11_build})

ADD_LIBRARY(json11_static STATIC IMPORTED)
SET_PROPERTY(TARGET json11_static PROPERTY IMPORTED_LOCATION ${json11_build}/${CMAKE_STATIC_LIBRARY_PREFIX}json11${CMAKE_STATIC_LIBRARY_SUFFIX})

MESSAGE(STATUS "ARGCV JSON11 STATIC " ${json11_build}/${CMAKE_STATIC_LIBRARY_PREFIX}json11${CMAKE_STATIC_LIBRARY_SUFFIX})

ADD_DEPENDENCIES(json11_static json11_proj)

SET(JSON11_LIBRARY_STATIC json11_static)




