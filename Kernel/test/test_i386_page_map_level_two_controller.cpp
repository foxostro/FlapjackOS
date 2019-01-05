#include "catch.hpp"
#include <platform/i386/page_map_level_one_controller.hpp>
#include <platform/i386/page_map_level_two_controller.hpp>
#include <common/test/mock_memory_management_unit.hpp>

using PML1 = i386::PageMapLevelOneController<MockMemoryManagementUnit>;
using PML2 = i386::PageMapLevelTwoController;

TEST_CASE("i386::PageMapLevelTwoController -- PML2 has 1024 entries", "[i386]")
{
    PML2 page_directory;
    REQUIRE(1024 == page_directory.get_number_of_entries());
}

TEST_CASE("i386::PageMapLevelTwoController -- page directory is initially empty", "[i386]")
{
    PML2 page_directory;
    for (size_t i = 0, n = page_directory.get_number_of_entries(); i < n; ++i) {
        auto& entry = page_directory.get_entry(i);
        REQUIRE(entry.get_pml1() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("i386::PageMapLevelTwoController -- ensure we can stick page tables in the directory", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory;

    {
        auto& entry = page_directory.get_entry(0);
        entry.set_pml1(new PML1{mmu});
    }

    {
        auto& entry = page_directory.get_entry(0);
        REQUIRE(entry.get_pml1() != nullptr);
        REQUIRE(entry.is_present() == true);
    }
}

TEST_CASE("i386::PageMapLevelTwoController -- ensure we can remove page tables from the directory", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory;
    uintptr_t addr = 0;

    {
        auto& entry = page_directory.get_entry(0);
        entry.set_pml1(new PML1{mmu});
        addr = entry.get_pml1()->get_page_table_physical_address();
    }

    {
        auto& entry = page_directory.get_entry(0);
        entry.set_pml1(nullptr);
    }

    {
        auto& entry = page_directory.get_entry(0);
        REQUIRE(entry.get_pml1() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}
