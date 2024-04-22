// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <c-utf8/convert.hpp>
#include <picobench/picobench.hpp>
#include <random>

namespace {

std::vector<char> generate(uint32_t size) {
    std::minstd_rand rnd(size);
    std::vector<char> result;
    result.reserve(size * 4);

    // generate random utf8 string with about 5:1 ascii:unicode ratio
    for (uint32_t i = 0; i < size; ++i) {
        if (rnd() % 5 != 0) {
            result.push_back(rnd() % 0x1F);
        }
        else {
            uint32_t c32;
            do {
                c32 = rnd() % (c_utf8::UTF32_MAX_CODEPOINT + 1);
            } while (c32 >= 0xFFF0 && c32 <= 0xFFFF); // skip specials
            auto str = c_utf8::utf32_to_utf8_str(c32);
            result.insert(result.end(), str.begin(), str.end());
        }
    }

    return result;
}

int utf8_buf_to_utf32_char_copy(uint32_t* out_char32, const char* utf8_buf, const char* utf8_buf_end) {
    char s[4] = C_UTF8_EMPTY_VAL;
    int i = 0;
    const char* ptr = utf8_buf;
    while (i < 4 && ptr != utf8_buf_end) {
        s[i++] = *ptr++;
    }
    return c_utf8::capi::c_utf8_buf_to_utf32_char_b(out_char32, s, nullptr);
}

int utf8_buf_to_utf32_char_reuse(uint32_t* out_char32, const char* utf8_buf, const char* utf8_buf_end) {
    char s[4] = C_UTF8_EMPTY_VAL;
    if (utf8_buf_end - utf8_buf < 4) {
        int i = 0;
        const char* ptr = utf8_buf;
        while (ptr != utf8_buf_end) {
            s[i++] = *ptr++;
        }
        utf8_buf = s;
    }
    return c_utf8::capi::c_utf8_buf_to_utf32_char_b(out_char32, utf8_buf, nullptr);
}

template <int (*Func)(uint32_t*, const char*, const char*)>
void benchmark(picobench::state& s) {
    auto data = generate(s.iterations());
    uint32_t sum = 0;

    std::string_view input(data.data(), data.size());
    {
        picobench::scope scope(s);
        while (!input.empty()) {
            uint32_t c32;
            auto len = Func(&c32, input.data(), input.data() + input.size());
            sum += c32;
            input.remove_prefix(len);
        }
    }

    s.set_result(sum);
}
}

PICOBENCH(benchmark<utf8_buf_to_utf32_char_copy>).label("copy");
PICOBENCH(benchmark<utf8_buf_to_utf32_char_reuse>).label("reuse");
