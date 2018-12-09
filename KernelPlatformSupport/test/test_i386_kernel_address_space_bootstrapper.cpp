#include "catch.hpp"
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <logical_addressing.hpp> // for convert_logical_to_physical_address()
#include "mock_memory_management_unit.hpp"

TEST_CASE("test_i386_kernel_address_space_bootstrapper", "[i386]")
{
    // Setup
    i386::PageDirectory pd;
    memset(&pd, 0, sizeof(pd));
    
    i386::PagingResolver resolver;
    resolver.set_cr3(convert_logical_to_physical_address((uintptr_t)&pd));

    MockMemoryManagementUnit mmu;
    mmu.set_cr3(resolver.get_cr3());
    i386::KernelAddressSpaceBootstrapper bootstrapper;

    // Action
    bootstrapper.prepare_address_space(mmu);

    // Test
    constexpr uintptr_t FOUR_MEGS = 0x400000;
    uintptr_t linear_address = (uintptr_t)KERNEL_VIRTUAL_START_ADDR;
    for (uintptr_t length = KERNEL_MEMORY_REGION_SIZE;
         length > 0;
         length -= FOUR_MEGS) {

        i386::PageTable* pt = resolver.get_page_table(linear_address);
        REQUIRE(pt != nullptr);

        linear_address += FOUR_MEGS;
    }
}
