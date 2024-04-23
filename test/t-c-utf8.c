// Copyright (c) Borislav Stanimirov
// SPDX-License-Identifier: MIT
//
#include <c-utf8/convert.h>

#include <unity.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

void setUp(void) {}
void tearDown(void) {}

void test_utf8_char_to_buf(void) {
    c_utf8_char_t in[5] = {0};
    for (int i = 0; i < 5; ++i) {
        for (int j = 0; j < i; ++j) {
            in[i].buf[j] = (char)j + 'a';
        }
        in[i].len = (uint16_t)i;
    }
    char out[5];
    const char empty[5] = {0};
    for (int i = 0; i < 5; ++i) {
        for (int o = 0; o < 5; ++o) {
            memset(out, 0, 5);
            int res = c_utf8_char_to_buf(out, out + o, in[i]);
            if (i > o) {
                TEST_ASSERT_EQUAL(0, res);
                TEST_ASSERT_EQUAL_CHAR_ARRAY(empty, out, 5);
            }
            else {
                TEST_ASSERT_EQUAL(i, res);
                TEST_ASSERT_EQUAL_STRING(in[i].buf, out);
            }
        }
    }
}

void test_conversion(const uint32_t c32) {
    const c_utf8_char_t c8 = c_utf32_char_to_utf8_char(c32);

    int err;
    uint32_t c32a = c_utf8_char_to_utf32_char(c8, &err);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(c32, c32a);

    // check convertion to utf8 buf
    char buf[5] = {0};
    int len = c_utf32_char_to_utf8_buf(buf, buf + 5, c32);
    TEST_ASSERT_EQUAL(c8.len, len);
    TEST_ASSERT_EQUAL_STRING(c8.buf, buf);

    // check error
    len = c_utf32_char_to_utf8_buf(buf, buf + c8.len - 1, c32);
    TEST_ASSERT_EQUAL(0, len);
    TEST_ASSERT_EQUAL_STRING(c8.buf, buf);

    // fill buff with 'x' so that it's not empty
    for (uint16_t i = c8.len; i < 5; ++i) {
        buf[i] = 'x';
    }

    // test min length
    len = c_utf8_buf_to_utf32_char(&c32a, buf, buf + c8.len, &err);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(c32, c32a);
    TEST_ASSERT_EQUAL(c8.len, len);

    // test past length
    len = c_utf8_buf_to_utf32_char(&c32a, buf, buf + 5, &err);
    TEST_ASSERT_EQUAL(0, err);
    TEST_ASSERT_EQUAL(c32, c32a);
    TEST_ASSERT_EQUAL(c8.len, len);
}

uint32_t rand32(void) {
    uint32_t ret;
    ret = rand() & 0xff;
    ret |= (rand() & 0xff) << 8;
    ret |= (rand() & 0xff) << 16;
    ret |= (rand() & 0xff) << 24;
    return ret;
}

uint32_t rand32_below(uint32_t max) {
    return rand32() % max;
}

void test_all(void) {
    for (uint32_t c32 = 1; c32 < 126; ++c32) {
        c_utf8_char_t c8 = c_utf32_char_to_utf8_char(c32);
        TEST_ASSERT_EQUAL(1, c8.len);
        TEST_ASSERT_EQUAL(c8.buf[0], c32);

        test_conversion(c32);
    }

    // fuzz sorta
    srand((unsigned)time(NULL));
    int i = 0;
    while (i < 100) {
        uint32_t c32 = rand32_below(C_UTF32_MAX_CODEPOINT + 1);
        if (c32 < 127) continue; // tested above
        if (c32 >= 0xFFF0 && c32 <= 0xFFFF) continue; // specials
        if (c32 >= 0xD800 && c32 <= 0xDFFF) continue; // surrogates
        test_conversion(c32);
        ++i;
    }
}

int main(void) {
    UNITY_BEGIN();
    test_utf8_char_to_buf();
    test_all();
    return UNITY_END();
}
