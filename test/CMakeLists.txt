# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.14 FATAL_ERROR)

include(./get_cpm.cmake)

CPMAddPackage(gh:iboB/doctest-util@0.1.1)
CPMAddPackage(gh:ThrowTheSwitch/Unity@2.5.2)

macro(cpp_test test)
    add_doctest_lib_test(${test} c-utf8 ${ARGN})
endmacro()

macro(c_test test)
    icm_add_test(
        NAME c-utf8-${test}
        TARGET test-c-utf8-${test}
        LIBRARIES
            unity::framework
            c-utf8::c-utf8
        SOURCES
            ${ARGN}
        FOLDER test
    )
endmacro()

c_test(c_core t-c-utf8.c)
cpp_test(cpp_core t-c-utf8.cpp)
