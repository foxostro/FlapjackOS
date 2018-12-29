#include "catch.hpp"
#include <common/elf32_parser.hpp>
#include <cstdio>

#define VERIFY_TEST_DATA 0

// I built an ELF32 executable image and then stuck it in this array.
// We'll use this as our test data.
static unsigned char g_test_data_32[] = {
#include "elf32_test_data.inc"
};

#if VERIFY_TEST_DATA
TEST_CASE("Elf32 Test data has expected contents.", "[Elf32]")
{
    size_t length = sizeof(g_test_data_32);
    FILE* fp = fopen("/Users/andrewfox/src/FlapjackOS/build/i386/User/Test1", "rb");
    fseek(fp, 0L, SEEK_END);
    size_t file_length = ftell(fp);
    rewind(fp);
    unsigned char* file_data = (unsigned char*)malloc(file_length);
    fread(file_data, file_length, 1, fp);
    fclose(fp);
    REQUIRE(length == file_length);
    REQUIRE(0 == memcmp(file_data, g_test_data_32, length));
    free(file_data);
}
#endif

TEST_CASE("Parser checks for ELF magic numbers", "[Elf32]")
{
    elf32::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};
    REQUIRE(parser.has_expected_magic());
}

TEST_CASE("Parser checks the image is appropriate for IA-32", "[Elf32]")
{
    elf32::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};
    REQUIRE(parser.is_ia32());
}
