#include "catch.hpp"
#include <platform/x86_64/page_table_entry.hpp>

// Memory layout of a page table entry:
// ----------------------------------------------------------------------------------------------
// | 63 | 62:59 | 58:52 | 51:M | (M-1):12  | 11:9 | 8 |  7  | 6 | 5 |  4  |  3  |  2  |  1  | 0 |
// |--------------------------------------------------------------------------------------------|
// | XD |   key |     x |    0 | phys addr |      | G | PAT | D | A | PCD | PWT | U/S | R/W | P |
// ----------------------------------------------------------------------------------------------

TEST_CASE("test_x86_64_page_table_entry_is_present_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PRESENT bit to zero
    REQUIRE(entry.is_present() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_present_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 1; // should set the PRESENT bit to one
    REQUIRE(entry.is_present() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_present_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 1; // should set the PRESENT bit to one
    entry.set_present(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_present_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PRESENT bit to zero
    entry.set_present(true);
    REQUIRE(entry.data == 1);
}

TEST_CASE("test_x86_64_page_table_entry_is_readwrite_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the READWRITE bit to zero
    REQUIRE(entry.is_readwrite() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_readwrite_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 2; // should set the READWRITE bit to one
    REQUIRE(entry.is_readwrite() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_readwrite_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 2; // should set the READWRITE bit to one
    entry.set_readwrite(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_readwrite_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the READWRITE bit to zero
    entry.set_readwrite(true);
    REQUIRE(entry.data == 2);
}

TEST_CASE("test_x86_64_page_table_entry_is_supervisor_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the SUPERVISOR bit to zero
    REQUIRE(entry.is_supervisor() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_supervisor_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 4; // should set the SUPERVISOR bit to one
    REQUIRE(entry.is_supervisor() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_supervisor_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 4; // should set the SUPERVISOR bit to one
    entry.set_supervisor(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_supervisor_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the SUPERVISOR bit to zero
    entry.set_supervisor(true);
    REQUIRE(entry.data == 4);
}

TEST_CASE("test_x86_64_page_table_entry_is_page_write_through_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PWT bit to zero
    REQUIRE(entry.is_page_write_through() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_page_write_through_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 8; // should set the PWT bit to one
    REQUIRE(entry.is_page_write_through() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_page_write_through_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 8; // should set the PWT bit to one
    entry.set_page_write_through(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_page_write_through_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PWT bit to zero
    entry.set_page_write_through(true);
    REQUIRE(entry.data == 8);
}

TEST_CASE("test_x86_64_page_table_entry_is_page_cache_disabled_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PCD bit to zero
    REQUIRE(entry.is_page_cache_disabled() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_page_cache_disabled_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 16; // should set the PCD bit to one
    REQUIRE(entry.is_page_cache_disabled() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_page_cache_disabled_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 16; // should set the PCD bit to one
    entry.set_page_cache_disabled(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_page_cache_disabled_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PCD bit to zero
    entry.set_page_cache_disabled(true);
    REQUIRE(entry.data == 16);
}

TEST_CASE("test_x86_64_page_table_entry_is_accessed_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the A bit to zero
    REQUIRE(entry.is_accessed() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_accessed_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 32; // should set the A bit to one
    REQUIRE(entry.is_accessed() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_accessed_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 32; // should set the A bit to one
    entry.set_accessed(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_accessed_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the A bit to zero
    entry.set_accessed(true);
    REQUIRE(entry.data == 32);
}

TEST_CASE("test_x86_64_page_table_entry_is_dirty_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the D bit to zero
    REQUIRE(entry.is_dirty() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_dirty_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1 << 6); // should set the D bit to one
    REQUIRE(entry.is_dirty() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_dirty_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1 << 6); // should set the D bit to one
    entry.set_dirty(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_dirty_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the D bit to zero
    entry.set_dirty(true);
    REQUIRE(entry.data == (1 << 6));
}

TEST_CASE("test_x86_64_page_table_entry_is_pat_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PAT bit to zero
    REQUIRE(entry.is_pat_bit_set() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_pat_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1 << 7); // should set the PAT bit to one
    REQUIRE(entry.is_pat_bit_set() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_pat_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1 << 7); // should set the PAT bit to one
    entry.set_pat_bit(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_pat_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the PAT bit to zero
    entry.set_pat_bit(true);
    REQUIRE(entry.data == (1 << 7));
}

TEST_CASE("test_x86_64_page_table_entry_is_global_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the G bit to zero
    REQUIRE(entry.is_global() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_global_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1 << 8); // should set the G bit to one
    REQUIRE(entry.is_global() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_global_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1 << 8); // should set the G bit to one
    entry.set_global(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_global_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the G bit to zero
    entry.set_global(true);
    REQUIRE(entry.data == (1 << 8));
}

TEST_CASE("test_x86_64_page_table_entry_is_execute_disabled_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the XD bit to zero
    REQUIRE(entry.is_execute_disabled() == false);
}

TEST_CASE("test_x86_64_page_table_entry_is_execute_disabled_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1ull << 63); // should set the XD bit to one
    REQUIRE(entry.is_execute_disabled() == true);
}

TEST_CASE("test_x86_64_page_table_entry_set_execute_disabled_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = (1ull << 63); // should set the XD bit to one
    entry.set_execute_disabled(false);
    REQUIRE(entry.data == 0);
}

TEST_CASE("test_x86_64_page_table_entry_set_execute_disabled_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0; // should set the XD bit to zero
    entry.set_execute_disabled(true);
    REQUIRE(entry.data == (1ull << 63));
}

TEST_CASE("test_x86_64_page_table_entry_set_addr_0", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0b1111111111111111111111111111111111111111111111111111111111111111;
    entry.set_address(0);
    REQUIRE(entry.data == 0b1111111111110000000000000000000000000000000000000000111111111111);
}

TEST_CASE("test_x86_64_page_table_entry_set_addr_1", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0;
    entry.set_address(~0ull);
    REQUIRE(entry.data == 0b0000000000001111111111111111111111111111111111111111000000000000);
}

TEST_CASE("test_x86_64_page_table_entry_get_address", "[x86_64]")
{
    x86_64::PageTableEntry entry;
    entry.data = 0b0000000000001111111111111111111111111111111111111111000000000000;
    REQUIRE(entry.get_address() == 0b1111111111111111111111111111111111111111);
}
