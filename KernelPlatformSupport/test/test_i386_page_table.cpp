#include "catch.hpp"

#include <platform/i386/page_table.hpp>
#include <platform/i386/protection_flags.hpp>

TEST_CASE("test_i386_page_table_index_for_virtual_address_0", "[i386]")
{
    i386::PageTable page_table;
    uintptr_t address = 0;
    size_t index = page_table.index_for_virtual_address(address);
    REQUIRE(index == 0);
}

TEST_CASE("test_i386_page_table_index_for_virtual_address_1", "[i386]")
{
    i386::PageTable page_table;
    uintptr_t address = 0xFFFFFFFF;
    size_t index = page_table.index_for_virtual_address(address);
    REQUIRE(index == 1023);
}
