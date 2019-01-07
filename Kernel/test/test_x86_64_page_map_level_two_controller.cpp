#include "catch.hpp"
#include <platform/x86_64/page_map_level_one_controller.hpp>
#include <platform/x86_64/page_map_level_two_controller.hpp>
#include "mock_memory_management_unit.hpp"

using PML1 = x86_64::PageMapLevelOneController;
using PML2 = x86_64::PageMapLevelTwoController;

TEST_CASE("x86_64::PageMapLevelTwoController -- PML2 has 512 entries", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory{mmu};
    REQUIRE(512 == page_directory.get_number_of_entries());
}

TEST_CASE("i386::PageMapLevelTwoController -- PML2 entry governs 2MB", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory{mmu};
    REQUIRE((2*1024*1024) == page_directory.get_size_governed_by_entry());
}

TEST_CASE("x86_64::PageMapLevelTwoController -- page directory is initially empty", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory{mmu};
    for (size_t i = 0, n = page_directory.get_number_of_entries(); i < n; ++i) {
        auto& entry = page_directory.get_entry(i);
        REQUIRE(entry.get_pml1() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("x86_64::PageMapLevelTwoController -- ensure we can stick page tables in the directory", "[x86_64]")
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

TEST_CASE("x86_64::PageMapLevelTwoController -- ensure we can remove page tables from the directory", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML2 page_directory{mmu};
    uintptr_t addr = 0;

    {
        auto& entry = page_directory.get_entry(0);
        entry.set_pml1(new PML1{mmu});
        addr = entry.get_pml1()->get_underlying_object_physical_address();
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

TEST_CASE("x86_64::PageMapLevelTwoController -- ctor can accept a raw pointer to a page directory", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    x86_64::PageDirectory* page_directory = new x86_64::PageDirectory;
    memset(page_directory, 0, sizeof(x86_64::PageDirectory));
    PML2 controller{mmu, page_directory};
    REQUIRE(controller.get_page_directory_pointer() == static_cast<void*>(page_directory));
}

TEST_CASE("x86_64::PageMapLevelTwoController -- ctor will not modify a page directory that it receives", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    x86_64::PageDirectory* actual = new x86_64::PageDirectory;
    memset(actual, 0xAB, sizeof(x86_64::PageDirectory));
    x86_64::PageDirectory* expected = new x86_64::PageDirectory;
    memcpy(expected, actual, sizeof(x86_64::PageDirectory));
    PML2 controller{mmu, actual};
    REQUIRE(0 == memcmp(expected, actual, sizeof(x86_64::PageDirectory)));
}

TEST_CASE("x86_64::PageMapLevelTwoController -- populate underlying page tables on request", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML2 pml2{mmu};
    pml2.populate(0);
    auto& entry = pml2.get_entry(0);
    REQUIRE(entry.get_pml1() != nullptr);
    REQUIRE(pml2.get_pml1_entry_by_address(0).is_present() == false);
}