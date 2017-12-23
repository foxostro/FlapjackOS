#include "catch.hpp"

#include "flapjack_libc/string.h"

#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <cassert>
#include <cstring>

TEST_CASE("test_strlen", "[String]")
{
    const char str[] = "0123456789";
    size_t len = STRLEN(str);
    REQUIRE(len == 10);
}

// When the string is NULL then STRLEN returns a length of zero.
TEST_CASE("test_strlen_null", "[String]")
{
    size_t len = STRLEN(NULL);
    REQUIRE(len == 0);
}

TEST_CASE("test_strnlen", "[String]")
{
    const char str[] = "0123456789";
    size_t len = STRNLEN(str, sizeof(str));
    REQUIRE(len == 10);
}

// When the string is NULL then STRNLEN returns a length of zero.
TEST_CASE("test_strnlen_null", "[String]")
{
    size_t len = STRNLEN(NULL, 0);
    REQUIRE(len == 0);
}

// STRNLEN will not exceed the specified character count while examining the
// string.
TEST_CASE("test_strnlen_exceed_the_count", "[String]")
{
    const char str[] = "0123456789";
    size_t len = STRNLEN(str, 4);
    REQUIRE(len == 4);
}

// Test that MEMCPY copies the entirety of the source buffer.
TEST_CASE("test_memcpy", "[String]")
{
    const char src[] = "source string here";
    char dst[sizeof(src)] = {0};

    void *r = MEMCPY(dst, src, strlen(src));
    REQUIRE(std::string(src) == std::string(dst));
    REQUIRE(std::string(src) == std::string("source string here"));
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// When size is zero then MEMCPY does nothing.
TEST_CASE("test_memcpy_zerolen", "[String]")
{
    const char src[] = "source string here!!";
    char dst[] = "buffer buffer buffer";

    void *r = MEMCPY(dst, src, 0);
    REQUIRE(std::string(dst) == "buffer buffer buffer");
    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string((char *)r) == dst);
}

// When size is zero and buffers are NULL then MEMCPY does nothing.
// So long as size is zero, we expect MEMCPY to do nothing and accept a NULL
// src and/or a NULL dst.
TEST_CASE("test_memcpy_zerolen_null", "[String]")
{
    void *r = MEMCPY(NULL, NULL, 0);
    REQUIRE(r == NULL);
}

// Test that MEMMOVE copies the buffers correctly when buffers do not overlap.
TEST_CASE("test_memmove_non_overlapping", "[String]")
{
    const char src[] = "source string here";
    char dst[sizeof(src)] = {0};

    void *r = MEMMOVE(dst, src, strlen(src));
    REQUIRE(std::string(src) == std::string(dst));
    REQUIRE(std::string(src) == std::string("source string here"));
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// When the buffers are the same address then MEMMOVE does nothing.
TEST_CASE("test_memmove_same", "[String]")
{
    const char expected[] = "source string here";
    char buffer[] = "source string here";
    const char *src = buffer;
    char *dst = buffer;

    void *r = MEMMOVE(dst, src, strlen(expected));
    REQUIRE(std::string(dst) == std::string(expected));
    REQUIRE(std::string(src) == std::string(expected));
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// Test that MEMMOVE copies the buffers correctly when buffers are overlapping
// and the dst buffer comes before the source buffer.
TEST_CASE("test_memmove_overlapping_before", "[String]")
{
    char buffer[] = "source string here!!";
    const char *src = buffer + 7;
    char *dst = buffer;

    REQUIRE(std::string(src) == "string here!!");
    REQUIRE(std::string(dst) == "source string here!!");
    void *r = MEMMOVE(dst, src, strlen("string here!!"));
    REQUIRE(std::string(dst) == "string here!! here!!");
    REQUIRE(std::string(src) == "here!! here!!");
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// Test that MEMMOVE copies the buffers correctly when buffers are overlapping
// and the dst buffer comes after the source buffer.
TEST_CASE("test_memmove_overlapping_after", "[String]")
{
    char buffer[] = "source string here!!";
    const char *src = buffer;
    char *dst = buffer + 7;

    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string(dst) == "string here!!");
    void *r = MEMMOVE(dst, src, strlen("source string"));
    REQUIRE(std::string(dst) == "source string");
    REQUIRE(std::string(src) == "source source string");
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// Test that MEMMOVE works correctly when size is zero.
TEST_CASE("test_memmove_zerolen", "[String]")
{
    char buffer[] = "source string here!!";
    const char *src = buffer;
    char *dst = buffer + 7;

    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string(dst) == "string here!!");
    void *r = MEMMOVE(dst, src, 0);
    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string(dst) == "string here!!");
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// When size is zero and buffers are NULL then MEMMOVE does nothing.
// So long as size is zero, we expect MEMMOVE to do nothing and accept a NULL
// src and/or a NULL dst.
TEST_CASE("test_memmove_zerolen_null", "[String]")
{
    void *r = MEMMOVE(NULL, NULL, 0);
    REQUIRE(r == NULL);
}

// Test that MEMSET correctly sets the buffer values.
TEST_CASE("test_memset", "[String]")
{
    char buffer[] = "buffer buffer buffer";
    void *r = MEMSET(buffer, 'X', strlen("buffer"));
    REQUIRE(std::string(buffer) == "XXXXXX buffer buffer");
    REQUIRE(std::string((char *)r) == buffer);
}

// Test that MEMSET correctly sets the buffer values when length is zero.
TEST_CASE("test_memset_zerolen", "[String]")
{
    char buffer[] = "buffer buffer buffer";
    void *r = MEMSET(buffer, 'X', 0);
    REQUIRE(std::string(buffer) == "buffer buffer buffer");
    REQUIRE(std::string((char *)r) == buffer);
}

// When the length is zero and the buffer is NULL then MEMSET does nothing.
TEST_CASE("test_memset_zerolen_null", "[String]")
{
    void *r = MEMSET(NULL, '?', 0);
    REQUIRE(r == NULL);
}
