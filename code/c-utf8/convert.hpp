// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#define C_UTF8_CPP_NAMESPACE c_utf8::capi
#include "convert.h"

#include <string>
#include <string_view>

namespace c_utf8 {

inline constexpr uint32_t UTF32_MAX_CODEPOINT = capi::C_UTF32_MAX_CODEPOINT;
inline constexpr uint32_t UTF32_INVALID_CODEPOINT = capi::C_UTF32_INVALID_CODEPOINT;

using utf8_char_t = capi::c_utf8_char_t;

inline int utf8_char_to_buf(char* out_utf8_buf, const char* utf8_buf_end, utf8_char_t char_utf8) {
    return capi::c_utf8_char_to_buf(out_utf8_buf, utf8_buf_end, char_utf8);
}

inline uint32_t utf8_to_utf32(const utf8_char_t char_utf8, int* opt_error = nullptr) {
    return capi::c_utf8_char_to_utf32_char(char_utf8, opt_error);
}

inline int utf8_to_utf32(uint32_t* out_char32, const char* utf8_buf, const char* utf8_buf_end, int* opt_error = nullptr) {
    return capi::c_utf8_buf_to_utf32_char(out_char32, utf8_buf, utf8_buf_end, opt_error);
}

inline uint32_t utf8_to_utf32(std::string_view u8str, int* opt_error = nullptr) {
    uint32_t ret;
    utf8_to_utf32(&ret, u8str.data(), u8str.data() + u8str.size(), opt_error);
    return ret;
}

inline uint32_t fwd_utf8_to_utf32(std::string_view& inout_u8str, int* opt_error = nullptr) {
    uint32_t ret;
    auto len = utf8_to_utf32(&ret, inout_u8str.data(), inout_u8str.data() + inout_u8str.size(), opt_error);
    inout_u8str = inout_u8str.substr(size_t(len));
    return ret;
}

inline utf8_char_t utf32_to_utf8(const uint32_t char32) {
    return capi::c_utf32_char_to_utf8_char(char32);
}

inline int utf32_to_utf8(char* out_utf8_buf, const char* utf8_buf_end, const uint32_t char32) {
    return capi::c_utf32_char_to_utf8_buf(out_utf8_buf, utf8_buf_end, char32);
}

inline std::string utf32_to_utf8_str(const uint32_t char32) {
    auto c8 = utf32_to_utf8(char32);
    return std::string(c8.buf, c8.len);
}

}
