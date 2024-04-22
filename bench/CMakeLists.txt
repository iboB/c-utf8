# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
CPMAddPackage(gh:iboB/picobench@2.07)

add_library(picobench-main STATIC picobench-main.cpp)
set_target_properties(picobench-main PROPERTIES FOLDER bench)
target_link_libraries(picobench-main PUBLIC picobench::picobench)

function(c_utf8_benchmark name)
    cmake_parse_arguments(ARG "" "" "SOURCES;LIBRARIES" ${ARGN})

    set(tgt bench-c-utf8-${name})
    add_executable(${tgt})
    set_target_properties(${tgt} PROPERTIES FOLDER bench)
    target_sources(${tgt} PRIVATE
        ${ARG_UNPARSED_ARGUMENTS}
        ${ARG_SOURCES}
    )
    target_link_libraries(${tgt} PRIVATE
        c-utf8::c-utf8
        picobench-main
        ${ARG_LIBRARIES}
    )
    if (MSVC)
        # could we do something smarter here?
        target_compile_options(${tgt} PRIVATE -arch:AVX2)
    else()
        target_compile_options(${tgt} PRIVATE -march:native)
    endif()

    add_custom_target(
        run-${tgt}
        COMMAND ${tgt}
    )
    set_target_properties(run-${tgt} PROPERTIES FOLDER bench)
endfunction()

c_utf8_benchmark(strings b-strings.cpp)
