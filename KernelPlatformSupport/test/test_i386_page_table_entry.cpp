#include "catch.hpp"

#include <platform/i386/page_table_entry.hpp>
#include <platform/i386/protection_flags.hpp>

TEST_CASE("test_i386_page_table_entry_is_present", "[i386]")
{
    i386::PageTableEntry entry;
    entry.pte = 1;
    REQUIRE(entry.is_present() == true);
}

TEST_CASE("test_i386_page_table_entry_is_not_present", "[i386]")
{
    i386::PageTableEntry entry;
    entry.pte = 0;
    REQUIRE(entry.is_present() == false);
}

TEST_CASE("test_i386_page_table_entry_set_writable", "[i386]")
{
    i386::PageTableEntry entry;
    entry.pte = 0;
    entry.set_writable(true);
    REQUIRE((entry.pte & i386::PAGING_WRITABLE) == i386::PAGING_WRITABLE);
}

TEST_CASE("test_i386_page_table_entry_set_not_writable", "[i386]")
{
    i386::PageTableEntry entry;
    entry.pte = 0;
    entry.set_writable(true);
    entry.set_writable(false);
    REQUIRE((entry.pte & i386::PAGING_WRITABLE) != i386::PAGING_WRITABLE);
}

TEST_CASE("test_i386_page_table_entry_set_flags", "[i386]")
{
    i386::PageTableEntry entry;
    entry.pte = 0;
    entry.set_flags(i386::PAGING_WRITABLE | i386::PAGING_PRESENT);
    REQUIRE((entry.pte & i386::PAGING_WRITABLE) != 0);
    REQUIRE(entry.is_present() == true);
}

TEST_CASE("test_i386_page_table_entry_set_page_frame", "[i386]")
{
    const uintptr_t fake_page_frame_address = 0x80000000;
    i386::PageTableEntry entry;
    entry.pte = 0;
    entry.set_page_frame(fake_page_frame_address);
    REQUIRE(entry.get_page_frame() == fake_page_frame_address);
}
