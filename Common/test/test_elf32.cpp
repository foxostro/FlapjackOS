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
TEST_CASE("Elf32: Test data has expected contents.", "[Elf32]")
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

TEST_CASE("Elf32: Parser checks the image is appropriate for IA-32 executable", "[Elf32]")
{
    elf::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};
    REQUIRE(parser.is_ia32());
    REQUIRE(parser.is_executable());
}

TEST_CASE("Elf32: Parser checks the start address matches the test gold", "[Elf32]")
{
    elf::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};

    // This value was determined by running `objdump -x' on the original file.
    REQUIRE(0x08048054 == parser.get_start_address());
}

TEST_CASE("Elf32: Parser checks the image contains the expected number of sections", "[Elf32]")
{
    elf::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};
    REQUIRE(5 == parser.get_number_of_section_headers());
}

TEST_CASE("Elf32: Parser checks the image contains section headers of the expected size", "[Elf32]")
{
    elf::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};
    REQUIRE(parser.is_section_header_size_valid());
}

TEST_CASE("Elf32: Parser checks the first section header is the expected null header", "[Elf32]")
{
    elf::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};
    const elf::Elf32_Shdr& header = parser.get_section_header(0);
    REQUIRE(0 == header.sh_name);
    REQUIRE(std::string() == parser.get_section_name(header.sh_name));
    REQUIRE(elf::SHT_NULL == header.sh_type);
    REQUIRE(0 == header.sh_flags);
    REQUIRE(0 == header.sh_addr);
    REQUIRE(0 == header.sh_offset);
    REQUIRE(elf::SHN_UNDEF == header.sh_link);
    REQUIRE(0 == header.sh_info);
    REQUIRE(0 == header.sh_addralign);
    REQUIRE(0 == header.sh_entsize);
}

TEST_CASE("Elf32: Parser checks that program headers match the test gold", "[Elf32]")
{
    elf::Parser32 parser{sizeof(g_test_data_32), g_test_data_32};

    // These values were determined by running `objdump -x' on the original file.
    REQUIRE(1 == parser.get_number_of_program_headers());
    const elf::Elf32_Phdr& header = parser.get_program_header(0);
    REQUIRE(elf::PT_LOAD == header.p_type);
    REQUIRE(0x0 == header.p_offset);
    REQUIRE(0x8048000 == header.p_vaddr);
    REQUIRE(0x8048000 == header.p_paddr);
    REQUIRE(0x5a == header.p_filesz);
    REQUIRE(0x5a == header.p_memsz);
    REQUIRE((elf::PF_R | elf::PF_X) == header.p_flags);
    REQUIRE(4096 == header.p_align);
}