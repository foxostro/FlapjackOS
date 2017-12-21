#include "catch.hpp"

#include "flapjack_libc/stdio.h"

#include <cstddef>
#include <cstdio>
#include <cassert>

// SNPRINTF returns zero when the buffer is null
TEST_CASE("test_null_buffer", "[stdio]")
{
    int c = SNPRINTF(NULL, 1000, NULL);
    REQUIRE(c == 0);
}

// SNPRINTF returns zero when the format string is null
TEST_CASE("test_null_format", "[stdio]")
{
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), NULL);
    REQUIRE(c == 0);
}

// SNPRINTF returns zero when the size is zero
TEST_CASE("test_zero_length", "[stdio]")
{
    int c = SNPRINTF(NULL, 0, NULL);
    REQUIRE(c == 0);
}

// SNPRINTF copies the format string verbatim when there are no args
// (assuming enough space in the output buffer)
TEST_CASE("test_return_no_args_0", "[stdio]")
{
    const char fmt[] = "test test";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), fmt);
    REQUIRE(c == strlen(fmt));
    REQUIRE(std::string(buf) == std::string(fmt));

    // ensure behavior is the same as libc
    char libcbuf[32] = {0};
    int d = snprintf(libcbuf, sizeof(libcbuf), fmt);
    REQUIRE(std::string(buf) == std::string(libcbuf));
    REQUIRE(c == d);
}

// SNPRINTF copies the format string verbatim when there are no args
// If there's not enough space it truncates the string in the output buffer.
// The return value is the number of characters that would have been output had
// the buffer not been size restricted.
TEST_CASE("test_return_no_args_1", "[stdio]")
{
    const char fmt[] = "test test";
    char buf[5];
    int c = SNPRINTF(buf, sizeof(buf), fmt);
    REQUIRE(c == strlen(fmt));
    REQUIRE(std::string(buf) == "test");

    // ensure behavior is the same as libc
    char libcbuf[5];
    int d = snprintf(libcbuf, sizeof(libcbuf), fmt);
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

TEST_CASE("test_return_char_arg", "[stdio]")
{
    static const char expected[] = "test a";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %c", 'a');
    REQUIRE(c == strlen(expected));
    REQUIRE(std::string(buf) == expected);

    // ensure behavior is the same as libc
    char libcbuf[32];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %c", 'a');
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

TEST_CASE("test_return_format_percent", "[stdio]")
{
    static const char expected[] = "test %";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %%");
    REQUIRE(c == strlen(expected));
    REQUIRE(std::string(buf) == expected);

    // ensure behavior is the same as libc
    char libcbuf[32];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %%");
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

// SNPRINTF can format string arguments
TEST_CASE("test_string_arg_0", "[stdio]")
{
    static const char expected[] = "test foo";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %s", "foo");
    REQUIRE(std::string(buf) == expected);
    REQUIRE(c == strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[32];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %s", "foo");
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

// SNPRINTF can format string arguments
// If the string is NULL then we format as "NULL".
TEST_CASE("test_string_arg_1", "[stdio]")
{
    static const char expected[] = "test (null)";
    char buf[32];
    int c = SNPRINTF(buf, sizeof(buf), "test %s", NULL);
    REQUIRE(std::string(buf) == expected);
    REQUIRE(c == strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[32];
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %s", NULL);
    #pragma GCC diagnostic pop
    REQUIRE(std::string(buf) == libcbuf);
    REQUIRE(c == d);
}

// SNPRINTF can format string arguments
// If the output buffer cannot contain the entire string then it truncates.
TEST_CASE("test_string_arg_2", "[stdio]")
{
    static const char expected[] = "test";
    char buf[5];
    int c = SNPRINTF(buf, sizeof(buf), "test %s", NULL);
    REQUIRE(std::string(buf) == expected);
    REQUIRE(c == strlen("test (null)"));

    // ensure behavior is the same as libc
    char libcbuf[5];
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wformat"
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %s", NULL);
    #pragma GCC diagnostic pop
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

// SNPRINTF can format integer arguments
TEST_CASE("test_int_arg_0", "[stdio]")
{
    static const char expected[] = "test -123";
    char buf[16];
    int c = SNPRINTF(buf, sizeof(buf), "test %d", -123);
    REQUIRE(c == strlen(expected));
    REQUIRE(std::string(buf) == expected);

    // ensure behavior is the same as libc
    char libcbuf[16];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %d", -123);
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

// SNPRINTF can format integer arguments
// If the output buffer cannot contain the entire string then it truncates.
TEST_CASE("test_int_arg_1", "[stdio]")
{
    static const char expected[] = "test 1"; // truncated by small buffer
    char buf[7];
    int c = SNPRINTF(buf, sizeof(buf), "test %d", 12345);
    REQUIRE(std::string(buf) == expected);

    // The buffer wasn't long enough to hold the entire output string. This
    // return value is the number of characters (preceding the null terminator)
    // that would have been output had the buffer not been size restricted.
    REQUIRE(c == strlen("test 12345"));

    // ensure behavior is the same as libc
    char libcbuf[7];
    int d = snprintf(libcbuf, sizeof(libcbuf), "test %d", 12345);
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

// SNPRINTF can format hexadecimal integer arguments
TEST_CASE("test_hex_arg_0", "[stdio]")
{
    static const char expected[] = "0xdeadbeef";
    char buf[16];
    int c = SNPRINTF(buf, sizeof(buf), "0x%x", 0xdeadbeef);
    REQUIRE(std::string(buf) == expected);
    REQUIRE(c == strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[16];
    int d = snprintf(libcbuf, sizeof(libcbuf), "0x%x", 0xdeadbeef);
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

// SNPRINTF can format hexadecimal integer arguments
TEST_CASE("test_hex_arg_1", "[stdio]")
{
    static const char expected[] = "0xCAFE10";
    char buf[16];
    int c = SNPRINTF(buf, sizeof(buf), "0x%X", 0xcafe10);
    REQUIRE(std::string(buf) == expected);
    REQUIRE(c == strlen(expected));

    // ensure behavior is the same as libc
    char libcbuf[16];
    int d = snprintf(libcbuf, sizeof(libcbuf), "0x%X", 0xcafe10);
    REQUIRE(c == d);
    REQUIRE(std::string(buf) == libcbuf);
}

// SNPRINTF can format pointers arguments too
TEST_CASE("test_ptr_arg", "[stdio]")
{
    static const size_t N = 3;
    static const void *values[N] = {
        (void *)0xdeadbeef,
        (void *)0x00010000,
        (void *)0xffffffff
    };

    // We expect our %p to left-pad the pointer addresses with zeroes. Since we
    // build unit tests for a 64-bit host architecture, we need sixteen
    // characters for the address portion of the output.
    // This doesn't match the exact behavior of the libc on my system, but I
    // like it anyway.
    assert(sizeof(void*) == 8);
    static const char *expected[N] = {
        "ptr is 0x00000000deadbeef",
        "ptr is 0x0000000000010000",
        "ptr is 0x00000000ffffffff"
    };

    char buf[128];

    for (size_t i = 0; i < N; ++i) {
        int r = SNPRINTF(buf, sizeof(buf), "ptr is %p", values[i]);
        REQUIRE(std::string(buf) == expected[i]);
        REQUIRE(r == strlen(expected[i]));
    }
}
