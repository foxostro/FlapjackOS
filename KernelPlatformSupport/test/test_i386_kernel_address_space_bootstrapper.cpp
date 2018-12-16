#include "catch.hpp"
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include "mock_memory_management_unit.hpp"

TEST_CASE("test_i386_kernel_address_space_bootstrapper", "[i386]")
{
    // Setup
    i386::PageDirectory pd;
    memset(&pd, 0, sizeof(pd));
    
    MockMemoryManagementUnit32 mmu;

    {
        uint32_t fakePhysicalAddress = 0x100000 + 512*0x1000;
        mmu.register_address((uintptr_t)&pd, fakePhysicalAddress);
    }

    i386::PagingResolver resolver(mmu);
    uintptr_t cr3 = mmu.convert_logical_to_physical_address((uintptr_t)&pd);
    resolver.set_cr3(cr3);
    mmu.set_cr3(cr3);
    i386::KernelAddressSpaceBootstrapper<MockMemoryManagementUnit32> bootstrapper(mmu);

    for (size_t i = 0; i < bootstrapper.NUMBER_OF_PAGE_TABLES; ++i) {
        auto& page_table = bootstrapper.page_tables_[i];
        uintptr_t linearAddress = (uintptr_t)&page_table;
        uint32_t fakePhysicalAddress = 0x100000 + i*0x1000;
        mmu.register_address(linearAddress, fakePhysicalAddress);
    }

    // Action
    bootstrapper.prepare_address_space();

    // Test
    constexpr uintptr_t FOUR_MEGS = 0x400000;
    uintptr_t linear_address = mmu.get_kernel_virtual_start_address();
    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0;
         length -= FOUR_MEGS) {

        i386::PageTable* pt = resolver.get_page_table(linear_address);
        REQUIRE(pt != nullptr);

        linear_address += FOUR_MEGS;
    }
}
