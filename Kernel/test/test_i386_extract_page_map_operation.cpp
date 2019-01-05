#include "catch.hpp"
#include <platform/i386/extract_page_map_operation.hpp>
#include "mock_paging_context.hpp"

static void assert_kernel_pml1_entry_is_expected(
    uintptr_t linear_address,
    PagingTopology::PageMapLevelOneController::Entry& entry)
{
    assert_is_page_aligned(linear_address);
    REQUIRE(entry.is_present() == false);
    REQUIRE(entry.get_page_frame() == nullptr);
}

static void assert_kernel_pml1_is_expected(
    uintptr_t linear_address,
    SharedPointer<PagingTopology::PageMapLevelOneController> pml1)
{
    assert_is_page_aligned(linear_address);
    for (size_t i = 0, n = pml1->get_number_of_entries(); i < n; ++i) {
        assert_kernel_pml1_entry_is_expected(linear_address, pml1->get_entry(i));
        linear_address += PAGE_SIZE;
    }
}

static void assert_kernel_pml2_entry_is_expected(
    uintptr_t linear_address,
    PagingTopology::PageMapLevelTwoController::Entry& entry)
{
    INFO(linear_address);
    assert_is_page_aligned(linear_address);
    REQUIRE(entry.is_present() == true);
    assert_kernel_pml1_is_expected(linear_address, entry.get_pml1());
}

static bool should_linear_address_be_mapped(uintptr_t linear_address)
{
    return linear_address >= KERNEL_VIRTUAL_START_ADDR_32BIT &&
           linear_address < (KERNEL_VIRTUAL_START_ADDR_32BIT + KERNEL_MEMORY_REGION_SIZE);
}

static void assert_pml2_entry_is_expected(
    uintptr_t linear_address,
    PagingTopology::PageMapLevelTwoController::Entry& entry)
{
    assert_is_page_aligned(linear_address);
    if (should_linear_address_be_mapped(linear_address)) {
        assert_kernel_pml2_entry_is_expected(linear_address, entry);
    } else {
        REQUIRE(entry.is_present() == false);
    }
}

static void assert_pml2_is_expected(
    SharedPointer<PagingTopology::PageMapLevelTwoController> pml2)
{
    constexpr uintptr_t FOUR_MEGS = 0x400000;
    uintptr_t linear_address = 0x00000000;
    for (size_t i = 0, n = pml2->get_number_of_entries(); i < n; ++i) {
        assert_pml2_entry_is_expected(linear_address, pml2->get_entry(i));
        linear_address += FOUR_MEGS;
    }
}

TEST_CASE("i386::ExtractPageMapOperation", "[i386]")
{
    MockPagingContext context;
    i386::ExtractPageMapOperation operation{context.mmu_};
    auto pml2 = operation.extract();
    assert_pml2_is_expected(pml2);
}
