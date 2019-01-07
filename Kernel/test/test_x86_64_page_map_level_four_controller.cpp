#include "catch.hpp"
#include <platform/x86_64/page_map_level_one_controller.hpp>
#include <platform/x86_64/page_map_level_two_controller.hpp>
#include <platform/x86_64/page_map_level_three_controller.hpp>
#include <platform/x86_64/page_map_level_four_controller.hpp>
#include "mock_memory_management_unit.hpp"

using PML1 = x86_64::PageMapLevelOneController;
using PML2 = x86_64::PageMapLevelTwoController;
using PML3 = x86_64::PageMapLevelThreeController;
using PML4 = x86_64::PageMapLevelFourController;

TEST_CASE("x86_64::PageMapLevelFourController -- PML4 has 512 entries", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML4 pml4{mmu};
    REQUIRE(512 == pml4.get_number_of_entries());
}

TEST_CASE("i386::PageMapLevelFourController -- PML4 entry governs 512GB", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML4 pml4{mmu};
    REQUIRE((512ul*1024*1024*1024) == pml4.get_size_governed_by_entry());
}

TEST_CASE("x86_64::PageMapLevelFourController -- PML4 is initially empty", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML4 pml4{mmu};
    for (size_t i = 0, n = pml4.get_number_of_entries(); i < n; ++i) {
        auto& entry = pml4.get_entry(i);
        REQUIRE(entry.get_pml3() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("x86_64::PageMapLevelFourController -- ensure we can stick PDPTs in the PML4", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML4 pml4{mmu};

    {
        auto& entry = pml4.get_entry(0);
        entry.set_pml3(new PML3{mmu});
    }

    {
        auto& entry = pml4.get_entry(0);
        REQUIRE(entry.get_pml3() != nullptr);
        REQUIRE(entry.is_present() == true);
    }
}

TEST_CASE("x86_64::PageMapLevelFourController -- ensure we can remove page tables from the directory", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML4 pml4{mmu};

    {
        auto& entry = pml4.get_entry(0);
        entry.set_pml3(new PML3{mmu});
    }

    {
        auto& entry = pml4.get_entry(0);
        entry.set_pml3(nullptr);
    }

    {
        auto& entry = pml4.get_entry(0);
        REQUIRE(entry.get_pml3() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("x86_64::PageMapLevelFourController -- ctor can accept a raw pointer to a PML4", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    x86_64::PageMapLevelFour* underlying_object = new x86_64::PageMapLevelFour;
    memset(underlying_object, 0, sizeof(x86_64::PageMapLevelFour));
    PML4 controller{mmu, underlying_object};
    REQUIRE(controller.get_underlying_object_pointer() == static_cast<void*>(underlying_object));
}

TEST_CASE("x86_64::PageMapLevelFourController -- ctor will not modify a PML4 that it receives", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    x86_64::PageMapLevelFour* actual = new x86_64::PageMapLevelFour;
    memset(actual, 0xAB, sizeof(x86_64::PageMapLevelFour));
    x86_64::PageMapLevelFour* expected = new x86_64::PageMapLevelFour;
    memcpy(expected, actual, sizeof(x86_64::PageMapLevelFour));
    PML4 controller{mmu, actual};
    REQUIRE(0 == memcmp(expected, actual, sizeof(x86_64::PageMapLevelFour)));
}

TEST_CASE("x86_64::PageMapLevelFourController -- populate underlying page tables on request", "[x86_64]")
{
    MockMemoryManagementUnit mmu;
    PML4 pml4{mmu};
    pml4.populate(0);

    // Drill down to ensure paging objects were populated for all lower levels.

    auto pml3 = pml4.get_entry(0).get_pml3();
    REQUIRE(pml3 != nullptr);
    REQUIRE(pml4.get_pml3_entry_by_address(0).is_present() == true);
    
    auto pml2 = pml3->get_entry(0).get_pml2();
    REQUIRE(pml2 != nullptr);
    REQUIRE(pml3->get_pml2_entry_by_address(0).is_present() == true);

    auto pml1 = pml2->get_entry(0).get_pml1();
    REQUIRE(pml1 != nullptr);
    REQUIRE(pml2->get_pml1_entry_by_address(0).is_present() == false);
}