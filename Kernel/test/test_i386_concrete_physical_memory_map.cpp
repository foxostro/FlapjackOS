#include "catch.hpp"
#include <platform/i386/concrete_hardware_memory_management_unit.hpp>
#include <platform/i386/concrete_physical_memory_map.hpp>
#include "mock_paging_context.hpp"

TEST_CASE("i386::ConcretePhysicalMemoryMap::map_page -- basic example", "[i386]")
{
    // Showcase the most basic use of map_page. This maps a single page in the
    // kernel virtual memory region to a single page frame at the corresponding
    // physical address.

    // Setup
    MockPagingContext context;
    i386::ConcretePhysicalMemoryMap phys_map{context.mmu_};
    phys_map.reload();

    // Action
    phys_map.map_page(context.mmu_.convert_logical_to_physical_address(context.mmu_.get_kernel_virtual_start_address()),
                      context.mmu_.get_kernel_virtual_start_address(),
                      phys_map.WRITABLE | phys_map.GLOBAL);

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(context.mmu_.get_kernel_virtual_start_address());
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
    REQUIRE(pte->is_global() == true);
    REQUIRE(pte->get_address() == context.mmu_.convert_logical_to_physical_address(context.mmu_.get_kernel_virtual_start_address()));
}

TEST_CASE("i386::ConcretePhysicalMemoryMap::set_readonly -- zero size region", "[i386]")
{
    // Setting a zero-size region as read-only is effectively a no-op.

    // Setup
    MockPagingContext context;
    i386::ConcretePhysicalMemoryMap phys_map{context.mmu_};
    phys_map.reload();
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      context.mmu_.get_kernel_virtual_start_address(),
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      context.mmu_.get_kernel_virtual_start_address()+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(context.mmu_.get_kernel_virtual_start_address(),
                          context.mmu_.get_kernel_virtual_start_address());

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(context.mmu_.get_kernel_virtual_start_address());
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}

TEST_CASE("i386::ConcretePhysicalMemoryMap::set_readonly -- one-byte region region", "[i386]")
{
    // Memory access permission are set on a per-page basis. So, setting a one
    // byte long region as read-only changes the entire associated page.

    // Setup
    MockPagingContext context;
    i386::ConcretePhysicalMemoryMap phys_map{context.mmu_};
    phys_map.reload();
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      context.mmu_.get_kernel_virtual_start_address(),
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      context.mmu_.get_kernel_virtual_start_address()+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(context.mmu_.get_kernel_virtual_start_address(),
                          1 + context.mmu_.get_kernel_virtual_start_address());

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(context.mmu_.get_kernel_virtual_start_address());
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == false);

    pte = context.resolver_.get_page_table_entry(context.mmu_.get_kernel_virtual_start_address()+PAGE_SIZE);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}

TEST_CASE("i386::ConcretePhysicalMemoryMap::set_readonly -- one-page region region", "[i386]")
{
    // If we set a PAGE_SIZE region as read-only then the associated page is
    // marked as read-only. The next page is not, because the end of the range
    // is exclusive, e.g., defined as [BEGIN, END)

    // Setup
    MockPagingContext context;
    i386::ConcretePhysicalMemoryMap phys_map{context.mmu_};
    phys_map.reload();
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      context.mmu_.get_kernel_virtual_start_address(),
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      context.mmu_.get_kernel_virtual_start_address()+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(context.mmu_.get_kernel_virtual_start_address(),
                          context.mmu_.get_kernel_virtual_start_address()+PAGE_SIZE);

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(context.mmu_.get_kernel_virtual_start_address());
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == false);

    pte = context.resolver_.get_page_table_entry(context.mmu_.get_kernel_virtual_start_address()+PAGE_SIZE);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}
