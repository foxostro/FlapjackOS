#include "catch.hpp"

#include <platform/i386/page_directory_entry.hpp>
#include <platform/i386/protection_flags.hpp>

TEST_CASE("test_i386_page_directory_entry_is_present", "[i386]")
{
    i386::PageDirectoryEntry entry;
    entry.pde = 1;
    REQUIRE(entry.is_present() == true);
}

TEST_CASE("test_i386_page_directory_entry_is_not_present", "[i386]")
{
    i386::PageDirectoryEntry entry;
    entry.pde = 0;
    REQUIRE(entry.is_present() == false);
}

TEST_CASE("test_i386_page_directory_entry_set_page_table", "[i386]")
{
    const uint32_t fake_page_table_address = 0x80000000;
    i386::PageTable* const fake_page_table = (i386::PageTable*)((uintptr_t)fake_page_table_address);

    i386::PageDirectoryEntry entry;
    entry.pde = 0;
    entry.set_page_table(fake_page_table);

    i386::PageTable& page_table = entry.get_page_table();
    REQUIRE(&page_table == fake_page_table);
}

TEST_CASE("test_i386_page_directory_entry_set_flags", "[i386]")
{
    const uint32_t fake_page_table_address = 0x80000000;
    i386::PageTable* const fake_page_table = (i386::PageTable*)((uintptr_t)fake_page_table_address);

    i386::PageDirectoryEntry entry;
    entry.pde = 0;
    entry.set_page_table(fake_page_table);
    entry.set_flags(i386::PAGING_PRESENT);

    i386::PageTable& page_table = entry.get_page_table();
    REQUIRE(&page_table == fake_page_table);

    REQUIRE(entry.is_present() == true);
}
