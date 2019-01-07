#include "catch.hpp"
#include <platform/x86_64/page_map_level_one_controller.hpp>
#include <platform/x86_64/page_map_level_two_controller.hpp>
#include <platform/x86_64/page_map_level_three_controller.hpp>
#include "mock_memory_management_unit.hpp"

using PML1 = x86_64::PageMapLevelOneController;
using PML2 = x86_64::PageMapLevelTwoController;
using PML3 = x86_64::PageMapLevelThreeController;

TEST_CASE("x86_64::PageMapLevelThreeController -- PML3 has 512 entries", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML3 pml3{mmu};
    REQUIRE(512 == pml3.get_number_of_entries());
}

TEST_CASE("i386::PageMapLevelThreeController -- PML3 entry governs 1GB", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML3 pml3{mmu};
    REQUIRE((1*1024*1024*1024) == pml3.get_size_governed_by_entry());
}

TEST_CASE("x86_64::PageMapLevelThreeController -- PDPT is initially empty", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML3 pml3{mmu};
    for (size_t i = 0, n = pml3.get_number_of_entries(); i < n; ++i) {
        auto& entry = pml3.get_entry(i);
        REQUIRE(entry.get_pml2() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("x86_64::PageMapLevelThreeController -- ensure we can stick page directories in the PDPT", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML3 pml3{mmu};

    {
        auto& entry = pml3.get_entry(0);
        entry.set_pml2(new PML2{mmu});
    }

    {
        auto& entry = pml3.get_entry(0);
        REQUIRE(entry.get_pml2() != nullptr);
        REQUIRE(entry.is_present() == true);
    }
}

TEST_CASE("x86_64::PageMapLevelThreeController -- ensure we can remove page directories from the PDPT", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML3 pml3{mmu};

    {
        auto& entry = pml3.get_entry(0);
        entry.set_pml2(new PML2{mmu});
    }

    {
        auto& entry = pml3.get_entry(0);
        entry.set_pml2(nullptr);
    }

    {
        auto& entry = pml3.get_entry(0);
        REQUIRE(entry.get_pml2() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("x86_64::PageMapLevelThreeController -- ctor can accept a raw pointer to a PDPT", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    x86_64::PageDirectoryPointerTable* pdpt = new x86_64::PageDirectoryPointerTable;
    memset(pdpt, 0, sizeof(x86_64::PageDirectoryPointerTable));
    PML3 controller{mmu, pdpt};
    REQUIRE(controller.get_physical_object_pointer() == static_cast<void*>(pdpt));
}

TEST_CASE("x86_64::PageMapLevelThreeController -- ctor will not modify a PDPT that it receives", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    x86_64::PageDirectoryPointerTable* actual = new x86_64::PageDirectoryPointerTable;
    memset(actual, 0xAB, sizeof(x86_64::PageDirectoryPointerTable));
    x86_64::PageDirectoryPointerTable* expected = new x86_64::PageDirectoryPointerTable;
    memcpy(expected, actual, sizeof(x86_64::PageDirectoryPointerTable));
    PML3 controller{mmu, actual};
    REQUIRE(0 == memcmp(expected, actual, sizeof(x86_64::PageDirectoryPointerTable)));
}

TEST_CASE("x86_64::PageMapLevelThreeController -- populate underlying page tables on request", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML3 pml3{mmu};
    pml3.populate(0);

    // Drill down to ensure paging objects were populated for all lower levels.

    auto pml2 = pml3.get_entry(0).get_pml2();
    REQUIRE(pml2 != nullptr);
    REQUIRE(pml3.get_pml2_entry_by_address(0).is_present() == true);

    auto pml1 = pml2->get_entry(0).get_pml1();
    REQUIRE(pml1 != nullptr);
    REQUIRE(pml2->get_pml1_entry_by_address(0).is_present() == false);
}