# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
cmake_minimum_required(VERSION 3.16)

project(c-utf8
    LANGUAGES C CXX
)

if(CMAKE_SOURCE_DIR STREQUAL CMAKE_CURRENT_SOURCE_DIR)
    # dev_mode is used below to make life simpler for developers
    # it enables some configurations and the defaults for building tests and examples
    # which typically wouldn't be built if confy is a subdirectory of another project
    set(dev_mode ON)
else()
    set(dev_mode OFF)
endif()

option(C_UTF8_BUILD_TESTS "c-utf8: build tests" ${dev_mode})
option(C_UTF8_BUILD_EXAMPLES "c-utf8: build examples" ${dev_mode})

mark_as_advanced(C_UTF8_BUILD_TESTS C_UTF8_BUILD_EXAMPLES C_UTF8_BUILD_SCRATCH)

if(dev_mode)
    include(./dev.cmake)
endif()

add_subdirectory(code)

if(C_UTF8_BUILD_TESTS)
    enable_testing()
    add_subdirectory(test)
endif()

#~ if(C_UTF8_BUILD_EXAMPLES)
#~     add_subdirectory(example)
#~ endif()

