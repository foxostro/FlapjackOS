#include "catch.hpp"
#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <common/test/mock_memory_management_unit.hpp>

TEST_CASE("test_x86_64_kernel_address_space_bootstrapper", "[x86_64]")
{
    // Setup
    x86_64::PageMapLevelFour pml4;
    x86_64::PageDirectoryPointerTable pdpt;
    x86_64::PageDirectory pd;

    memset(&pml4, 0, sizeof(pml4));
    memset(&pdpt, 0, sizeof(pdpt));
    memset(&pd, 0, sizeof(pd));

    MockMemoryManagementUnit64 mmu;
    uintptr_t cr3 = mmu.convert_logical_to_physical_address((uintptr_t)&pml4);
    mmu.set_cr3(cr3);

    x86_64::PagingResolver<MockMemoryManagementUnit64> resolver(mmu);
    resolver.set_cr3(cr3);

    size_t pml4e_index = resolver.get_page_map_level_four_index(mmu.get_kernel_virtual_start_address());
    auto& pml4e = pml4.entries[pml4e_index];
    pml4e.set_address(mmu.convert_logical_to_physical_address((uintptr_t)&pdpt));

    size_t pdpte_index = resolver.get_page_directory_pointer_table_index(mmu.get_kernel_virtual_start_address());
    auto& pdpte = pdpt.entries[pdpte_index];
    pdpte.set_address(mmu.convert_logical_to_physical_address((uintptr_t)&pd));

    x86_64::KernelAddressSpaceBootstrapper<MockMemoryManagementUnit64> bootstrapper(mmu);

    // Action
    bootstrapper.prepare_address_space();

    // Test
    constexpr uintptr_t TWO_MEGS = 0x200000;
    uintptr_t linear_address = mmu.get_kernel_virtual_start_address();
    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0;
         length -= TWO_MEGS) {

        x86_64::PageTable* pt = resolver.get_page_table(linear_address);
        REQUIRE(pt != nullptr);

        linear_address += TWO_MEGS;
    }
}
