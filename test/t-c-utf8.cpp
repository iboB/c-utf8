// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <c-utf8/convert.hpp>

#include <doctest/doctest.h>

#include <random>

uint32_t fb(const char32_t* str) {
    return str[0];
}

using namespace c_utf8;

TEST_CASE("convert") {
    CHECK(utf8_to_utf32(u8"Щ") == fb(U"Щ"));
    CHECK(utf8_to_utf32(u8"→") == fb(U"→"));
    CHECK(utf8_to_utf32(u8"⚽") == fb(U"⚽"));
    CHECK(utf8_to_utf32(u8"⛄") == fb(U"⛄"));
    CHECK(utf8_to_utf32(u8"🔔") == fb(U"🔔"));
    CHECK(utf8_to_utf32(u8"∞") == fb(U"∞"));

    CHECK(utf32_to_utf8_str(fb(U"ѝ")) == u8"ѝ");
    CHECK(utf32_to_utf8_str(fb(U"ぽ")) == u8"ぽ");
    CHECK(utf32_to_utf8_str(fb(U"⊙")) == u8"⊙");
    CHECK(utf32_to_utf8_str(fb(U"🎨")) == u8"🎨");
    CHECK(utf32_to_utf8_str(fb(U"🙈")) == u8"🙈");
    CHECK(utf32_to_utf8_str(fb(U"🐓")) == u8"🐓");
}

TEST_CASE("fuzz") {
    std::minstd_rand rnd(std::random_device{}());
    for (int i = 0; i < 100; ++i) {
        uint32_t c32 = rnd() % (UTF32_MAX_CODEPOINT + 1);
        if (c32 >= 0xFFF0 && c32 <= 0xFFFF) continue; // specials
        auto str = utf32_to_utf8_str(c32);
        int err;
        auto c32a = utf8_to_utf32(str, &err);
        CHECK(!err);
        CHECK(c32 == c32a);
    }
}
