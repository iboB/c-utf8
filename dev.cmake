# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_C_STANDARD 99)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_C_STANDARD_REQUIRED ON)

set_property(GLOBAL PROPERTY USE_FOLDERS ON)

set(CMAKE_LINK_DEPENDS_NO_SHARED ON)

if(MSVC)
    # /Zc:preprocessor - incompatible with Windows.h
    # /Zc:templateScope - TODO: add when msvc 17.5 is the norm
    add_compile_options(
        /W4
        -D_CRT_SECURE_NO_WARNINGS /Zc:__cplusplus /permissive-
        /volatile:iso /Zc:throwingNew /utf-8 -DNOMINMAX=1
        /wd4251 /wd4275
    )
else()
    add_compile_options(-Wall -Wextra)
endif()

# sanitizers
option(SAN_THREAD "${CMAKE_PROJECT_NAME}: sanitize thread" OFF)
option(SAN_ADDR "${CMAKE_PROJECT_NAME}: sanitize address" OFF)
option(SAN_UB "${CMAKE_PROJECT_NAME}: sanitize undefined behavior" OFF)
option(SAN_LEAK "${CMAKE_PROJECT_NAME}: sanitize leaks" OFF)

if(MSVC)
    if(SAN_ADDR)
        add_compile_options(/fsanitize=address)
    endif()
    if(SAN_THREAD OR SAN_UB OR SAN_LEAK)
        message(WARNING "Unsupported sanitizers requested for msvc. Ignored")
    endif()
else()
    if(SAN_THREAD)
        set(icm_san_flags -fsanitize=thread -g)
        if(SAN_ADDR OR SAN_UB OR SAN_LEAK)
            message(WARNING "Incompatible sanitizer combination requested. Only 'SAN_THREAD' will be respected")
        endif()
    else()
        if(SAN_ADDR)
            list(APPEND icm_san_flags -fsanitize=address -pthread)
        endif()
        if(SAN_UB)
            list(APPEND icm_san_flags -fsanitize=undefined)
        endif()
        if(SAN_LEAK)
            if(APPLE)
                message(WARNING "Unsupported leak sanitizer requested for Apple. Ignored")
            else()
                list(APPEND icm_san_flags -fsanitize=leak)
            endif()
        endif()
    endif()
    if(icm_san_flags)
        add_compile_options(${icm_san_flags})
        add_link_options(${icm_san_flags})
    endif()
endif()

# all binaries to bin
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
