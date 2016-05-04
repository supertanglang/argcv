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
ADD_EXECUTABLE(ctest_argcv argcv_test.cc)

#SET(EXECUTABLE_OUTPUT_PATH ${PROJECT_BINARY_DIR}/test)
#MESSAGE(STATUS "ARGCV_EXECUTABLE_OUTPUT_PATH " ${EXECUTABLE_OUTPUT_PATH})

ADD_DEPENDENCIES(ctest_argcv argcv)

TARGET_LINK_LIBRARIES(ctest_argcv argcv_static)
TARGET_LINK_LIBRARIES(ctest_argcv argcv_string_static)
TARGET_LINK_LIBRARIES(ctest_argcv argcv_sys_static)
TARGET_LINK_LIBRARIES(ctest_argcv argcv_wrapper_leveldb_static)
TARGET_LINK_LIBRARIES(ctest_argcv argcv_wrapper_ini_static)
TARGET_LINK_LIBRARIES(ctest_argcv pthread)

#  executable properties
SET_TARGET_PROPERTIES(ctest_argcv PROPERTIES OUTPUT_NAME ctest_argcv)


ADD_TEST(argcv_info ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 0)
ADD_TEST(argcv_string_split ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 1)
ADD_TEST(argcv_string_replace ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 2)
ADD_TEST(argcv_string_uuid ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 3)
ADD_TEST(argcv_string_stemmer ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 4)
ADD_TEST(argcv_ir_index_analyzer_basic ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 5)
ADD_TEST(argcv_wrapper_leveldb_wr ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 6)
ADD_TEST(argcv_wrapper_ini_wr ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 7)
ADD_TEST(argcv_threads ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 8)
ADD_TEST(argcv_dir_trav ${EXECUTABLE_OUTPUT_PATH}/ctest_argcv 9)

