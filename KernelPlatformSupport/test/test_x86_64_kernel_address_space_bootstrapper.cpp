#include "catch.hpp"
#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <logical_addressing.hpp> // for convert_logical_to_physical_address()
#include "mock_memory_management_unit.hpp"

TEST_CASE("test_x86_64_kernel_address_space_bootstrapper", "[x86_64]")
{
    // Setup
    x86_64::PageMapLevelFour pml4;
    x86_64::PageDirectoryPointerTable pdpt;
    x86_64::PageDirectory pd;

    memset(&pml4, 0, sizeof(pml4));
    memset(&pdpt, 0, sizeof(pdpt));
    memset(&pd, 0, sizeof(pd));
    
    x86_64::PagingResolver resolver;
    resolver.set_cr3(convert_logical_to_physical_address((uintptr_t)&pml4));

    size_t pml4e_index = resolver.get_page_map_level_four_index(KERNEL_VIRTUAL_START_ADDR);
    auto& pml4e = pml4.entries[pml4e_index];
    pml4e.set_address(convert_logical_to_physical_address((uintptr_t)&pdpt));

    size_t pdpte_index = resolver.get_page_directory_pointer_table_index(KERNEL_VIRTUAL_START_ADDR);
    auto& pdpte = pdpt.entries[pdpte_index];
    pdpte.set_address(convert_logical_to_physical_address((uintptr_t)&pd));

    MockMemoryManagementUnit mmu;
    mmu.set_cr3(resolver.get_cr3());
    x86_64::KernelAddressSpaceBootstrapper bootstrapper;

    // Action
    bootstrapper.prepare_address_space(mmu);

    // Test
    constexpr uintptr_t TWO_MEGS = 0x200000;
    uintptr_t linear_address = (uintptr_t)KERNEL_VIRTUAL_START_ADDR;
    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0;
         length -= TWO_MEGS) {

        x86_64::PageTable* pt = resolver.get_page_table(linear_address);
        REQUIRE(pt != nullptr);

        linear_address += TWO_MEGS;
    }
}
