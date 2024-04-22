# Copyright (c) Borislav Stanimirov
# SPDX-License-Identifier: MIT
#
add_library(c-utf8 INTERFACE)
add_library(c-utf8::c-utf8 ALIAS c-utf8)
target_include_directories(c-utf8 INTERFACE .)
