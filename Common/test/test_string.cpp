#include "catch.hpp"

#include "flapjack_libc/cstring"

#include <cstdlib>
#include <cstdio>
#include <cstddef>
#include <cassert>
#include <cstring>

TEST_CASE("test_strlen", "[String]")
{
    const char str[] = "0123456789";
    size_t len = flapjack::strlen(str);
    REQUIRE(len == 10);
}

// When the string is NULL then strlen returns a length of zero.
TEST_CASE("test_strlen_null", "[String]")
{
    size_t len = flapjack::strlen(nullptr);
    REQUIRE(len == 0);
}

TEST_CASE("test_strnlen", "[String]")
{
    const char str[] = "0123456789";
    size_t len = flapjack::strnlen(str, sizeof(str));
    REQUIRE(len == 10);
}

// When the string is NULL then strnlen returns a length of zero.
TEST_CASE("test_strnlen_null", "[String]")
{
    size_t len = flapjack::strnlen(nullptr, 0);
    REQUIRE(len == 0);
}

// strnlen will not exceed the specified character count while examining the
// string.
TEST_CASE("test_strnlen_exceed_the_count", "[String]")
{
    const char str[] = "0123456789";
    size_t len = flapjack::strnlen(str, 4);
    REQUIRE(len == 4);
}

// Test that memcpy copies the entirety of the source buffer.
TEST_CASE("test_memcpy", "[String]")
{
    const char src[] = "source string here";
    char dst[sizeof(src)] = {0};

    void *r = flapjack::memcpy(dst, src, strlen(src));
    REQUIRE(std::string(src) == std::string(dst));
    REQUIRE(std::string(src) == std::string("source string here"));
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// When size is zero then memcpy does nothing.
TEST_CASE("test_memcpy_zerolen", "[String]")
{
    const char src[] = "source string here!!";
    char dst[] = "buffer buffer buffer";

    void *r = flapjack::memcpy(dst, src, 0);
    REQUIRE(std::string(dst) == "buffer buffer buffer");
    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string((char *)r) == dst);
}

// When size is zero and buffers are NULL then memcpy does nothing.
// So long as size is zero, we expect memcpy to do nothing and accept a NULL
// src and/or a NULL dst.
TEST_CASE("test_memcpy_zerolen_null", "[String]")
{
    void *r = flapjack::memcpy(nullptr, nullptr, 0);
    REQUIRE(r == nullptr);
}

// Test that memmove copies the buffers correctly when buffers do not overlap.
TEST_CASE("test_memmove_non_overlapping", "[String]")
{
    const char src[] = "source string here";
    char dst[sizeof(src)] = {0};

    void *r = flapjack::memmove(dst, src, strlen(src));
    REQUIRE(std::string(src) == std::string(dst));
    REQUIRE(std::string(src) == std::string("source string here"));
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// When the buffers are the same address then memmove does nothing.
TEST_CASE("test_memmove_same", "[String]")
{
    const char expected[] = "source string here";
    char buffer[] = "source string here";
    const char *src = buffer;
    char *dst = buffer;

    void *r = flapjack::memmove(dst, src, strlen(expected));
    REQUIRE(std::string(dst) == std::string(expected));
    REQUIRE(std::string(src) == std::string(expected));
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// Test that memmove copies the buffers correctly when buffers are overlapping
// and the dst buffer comes before the source buffer.
TEST_CASE("test_memmove_overlapping_before", "[String]")
{
    char buffer[] = "source string here!!";
    const char *src = buffer + 7;
    char *dst = buffer;

    REQUIRE(std::string(src) == "string here!!");
    REQUIRE(std::string(dst) == "source string here!!");
    void *r = flapjack::memmove(dst, src, strlen("string here!!"));
    REQUIRE(std::string(dst) == "string here!! here!!");
    REQUIRE(std::string(src) == "here!! here!!");
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// Test that memmove copies the buffers correctly when buffers are overlapping
// and the dst buffer comes after the source buffer.
TEST_CASE("test_memmove_overlapping_after", "[String]")
{
    char buffer[] = "source string here!!";
    const char *src = buffer;
    char *dst = buffer + 7;

    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string(dst) == "string here!!");
    void *r = flapjack::memmove(dst, src, strlen("source string"));
    REQUIRE(std::string(dst) == "source string");
    REQUIRE(std::string(src) == "source source string");
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// Test that memmove works correctly when size is zero.
TEST_CASE("test_memmove_zerolen", "[String]")
{
    char buffer[] = "source string here!!";
    const char *src = buffer;
    char *dst = buffer + 7;

    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string(dst) == "string here!!");
    void *r = flapjack::memmove(dst, src, 0);
    REQUIRE(std::string(src) == "source string here!!");
    REQUIRE(std::string(dst) == "string here!!");
    REQUIRE(std::string((char *)r) == std::string(dst));
}

// When size is zero and buffers are NULL then memmove does nothing.
// So long as size is zero, we expect memmove to do nothing and accept a NULL
// src and/or a NULL dst.
TEST_CASE("test_memmove_zerolen_null", "[String]")
{
    void *r = flapjack::memmove(nullptr, nullptr, 0);
    REQUIRE(r == nullptr);
}

// Test that memset correctly sets the buffer values.
TEST_CASE("test_memset", "[String]")
{
    char buffer[] = "buffer buffer buffer";
    void *r = flapjack::memset(buffer, 'X', strlen("buffer"));
    REQUIRE(std::string(buffer) == "XXXXXX buffer buffer");
    REQUIRE(std::string((char *)r) == buffer);
}

// Test that memset correctly sets the buffer values when length is zero.
TEST_CASE("test_memset_zerolen", "[String]")
{
    char buffer[] = "buffer buffer buffer";
    void *r = flapjack::memset(buffer, 'X', 0);
    REQUIRE(std::string(buffer) == "buffer buffer buffer");
    REQUIRE(std::string((char *)r) == buffer);
}

// When the length is zero and the buffer is NULL then memset does nothing.
TEST_CASE("test_memset_zerolen_null", "[String]")
{
    void *r = flapjack::memset(nullptr, '?', 0);
    REQUIRE(r == nullptr);
}

TEST_CASE("test strtol -- pass null string", "[String]")
{
    REQUIRE(0 == flapjack::strtol(nullptr, nullptr, -1));
}

TEST_CASE("test strtol -- strtol(\"ZZ\", nullptr, 10)", "[String]")
{
    // Invalid characters in string.
    REQUIRE(0 == flapjack::strtol("ZZ", nullptr, 10));
}

TEST_CASE("test strtol -- strtol(\"0\", nullptr, 10)", "[String]")
{
    REQUIRE(0 == flapjack::strtol("0", nullptr, 10));
}

TEST_CASE("test strtol -- strtol(\"-1\", nullptr, 10)", "[String]")
{
    REQUIRE(-1 == flapjack::strtol("-1", nullptr, 10));
}

TEST_CASE("test strtol -- strtol(\"+1\", nullptr, 10)", "[String]")
{
    REQUIRE(1 == flapjack::strtol("+1", nullptr, 10));
}

TEST_CASE("test strtol -- strtol(10, nullptr, 10)", "[String]")
{
    REQUIRE(10 == flapjack::strtol("10", nullptr, 10));
}

TEST_CASE("test strtol -- strtol(10, nullptr, 0)", "[String]")
{
    REQUIRE(10 == flapjack::strtol("10", nullptr, 0));
}

TEST_CASE("test strtol -- strtol(-123, nullptr, 10)", "[String]")
{
    REQUIRE(-123 == flapjack::strtol("-123", nullptr, 10));
}

TEST_CASE("test strtol -- strtol(\"10\", nullptr, 16)", "[String]")
{
    REQUIRE(0x10 == flapjack::strtol("10", nullptr, 16));
}

TEST_CASE("test strtol -- strtol(\"Ff\", nullptr, 16)", "[String]")
{
    REQUIRE(0xff == flapjack::strtol("Ff", nullptr, 16));
}

TEST_CASE("test strtol -- strtol(\"10\", nullptr, 8)", "[String]")
{
    REQUIRE(8 == flapjack::strtol("10", nullptr, 8));
}

TEST_CASE("test strtol -- strtol(\"123foo\", nullptr, 10)", "[String]")
{
    REQUIRE(123 == flapjack::strtol("123foo", nullptr, 10));
}

TEST_CASE("test strtol -- strtol(\"123foo\", non-null, 10)", "[String]")
{
    const char* s = "123foo";
    char* endptr = nullptr;
    REQUIRE(123 == flapjack::strtol(s, &endptr, 10));
    REQUIRE(endptr == s + 3);
}

TEST_CASE("test strtol -- strtol(\"+_\", non-null, 0)", "[String]")
{
    const char* s = "+_";
    char* endptr = nullptr;

    REQUIRE(0 == flapjack::strtol(s, &endptr, 0));
    REQUIRE(endptr == s);
}

TEST_CASE("test strtol -- strtol(\"0x10\", nullptr, 16)", "[String]")
{
    REQUIRE(0x10 == flapjack::strtol("0x10", nullptr, 16));
}

TEST_CASE("test strtol -- strtol(\"0x10\", nullptr, 0)", "[String]")
{
    REQUIRE(0x10 == flapjack::strtol("0x10", nullptr, 0));
}

TEST_CASE("test strtol -- strtol(\"010\", nullptr, 8)", "[String]")
{
    REQUIRE(010 == flapjack::strtol("010", nullptr, 8));
}

TEST_CASE("test strtol -- strtol(\"0x\", nullptr, 0)", "[String]")
{
    REQUIRE(0 == flapjack::strtol("0x", nullptr, 0));
}

TEST_CASE("test strtol -- strtol(\"0x\", non-null, 0)", "[String]")
{
    const char* s = "0x";
    char* endptr = nullptr;
    REQUIRE(0 == flapjack::strtol(s, &endptr, 0));
    REQUIRE(endptr == s);
}

TEST_CASE("test strtol -- strtol(\"123\", nullptr, -1)", "[String]")
{
    REQUIRE(0 == flapjack::strtol("123", nullptr, -1));
}

TEST_CASE("test strtol -- strtol(\"123\", nullptr, 36)", "[String]")
{
    REQUIRE(0 == flapjack::strtol("123", nullptr, 36));
}
