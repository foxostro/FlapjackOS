#include "catch.hpp"

#include <platform/i386/page_directory.hpp>
#include <platform/i386/page_table.hpp>
#include <platform/i386/protection_flags.hpp>

TEST_CASE("test_i386_page_directory_index_for_virtual_address_0", "[i386]")
{
    i386::PageDirectory pd;
    uintptr_t address = 0;
    size_t index = pd.index_for_virtual_address(address);
    REQUIRE(index == 0);
}

TEST_CASE("test_i386_page_directory_index_for_virtual_address_1", "[i386]")
{
    i386::PageDirectory pd;
    uintptr_t address = 0xFFFFFFFF;
    size_t index = pd.index_for_virtual_address(address);
    REQUIRE(index == 1023);
}

TEST_CASE("test_i386_page_directory_is_address_mapped_0", "[i386]")
{
    i386::PageDirectory pd;
    REQUIRE(pd.is_address_mapped(0x00000000) == false);
    REQUIRE(pd.is_address_mapped(0xFFFFFFFF) == false);
}

TEST_CASE("test_i386_page_directory_map_page", "[i386]")
{
    i386::PageTable pt;
    memset(pt.entries, 0, sizeof(pt.entries));

    i386::PageDirectory pd;
    memset(pd.entries, 0, sizeof(pd.entries));   
    pd.get_pde(0xC0000000).set_page_table(&pt);

    pd.map_page(0xC0000000, 0xC0000000, 0);

    REQUIRE(pd.is_address_mapped(0xC0000000) == true);
}
