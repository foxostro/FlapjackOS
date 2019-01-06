#include "catch.hpp"
#include <platform/i386/page_map_level_one_controller.hpp>
#include <platform/i386/page_map_level_two_controller.hpp>
#include "mock_memory_management_unit.hpp"

using PML1 = i386::PageMapLevelOneController;
using PML2 = i386::PageMapLevelTwoController;

TEST_CASE("i386::PageMapLevelTwoController -- PML2 has 1024 entries", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory{mmu};
    REQUIRE(1024 == page_directory.get_number_of_entries());
}

TEST_CASE("i386::PageMapLevelTwoController -- page directory is initially empty", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory{mmu};
    for (size_t i = 0, n = page_directory.get_number_of_entries(); i < n; ++i) {
        auto& entry = page_directory.get_entry(i);
        REQUIRE(entry.get_pml1() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("i386::PageMapLevelTwoController -- ensure we can stick page tables in the directory", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory{mmu};

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
    PML2 page_directory{mmu};
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

TEST_CASE("i386::PageMapLevelTwoController -- ctor can accept a raw pointer to a page directory", "[i386]")
{
    MockMemoryManagementUnit mmu;
    i386::PageDirectory* page_directory = new i386::PageDirectory;
    memset(page_directory, 0, sizeof(i386::PageDirectory));
    PML2 controller{mmu, page_directory};
    REQUIRE(controller.get_page_directory_pointer() == static_cast<void*>(page_directory));
}

TEST_CASE("i386::PageMapLevelTwoController -- ctor will not modify a page directory that it receives", "[i386]")
{
    MockMemoryManagementUnit mmu;
    i386::PageDirectory* actual = new i386::PageDirectory;
    memset(actual, 0xAB, sizeof(i386::PageDirectory));
    i386::PageDirectory* expected = new i386::PageDirectory;
    memcpy(expected, actual, sizeof(i386::PageDirectory));
    PML2 controller{mmu, actual};
    REQUIRE(0 == memcmp(expected, actual, sizeof(i386::PageDirectory)));
}

TEST_CASE("i386::PageMapLevelTwoController -- populate underlying page tables on request", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML2 pml2{mmu};
    pml2.populate(0);
    auto& entry = pml2.get_entry(0);
    REQUIRE(entry.get_pml1() != nullptr);
    REQUIRE(pml2.get_pml1_entry_by_offset(0).is_present() == false);
}