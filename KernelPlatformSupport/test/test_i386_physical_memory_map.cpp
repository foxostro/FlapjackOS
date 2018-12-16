#include "catch.hpp"
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <platform/i386/physical_memory_map.hpp>
#include <logical_addressing.hpp> // for convert_logical_to_physical_address()
#include "mock_memory_management_unit.hpp"

class TestPagingContext
{
public:
    i386::PageDirectory page_directory_;
    MockMemoryManagementUnit mmu_;
    i386::PagingResolver resolver_;
    i386::KernelAddressSpaceBootstrapper bootstrapper_;

    TestPagingContext()
    {   
        memset(&page_directory_, 0, sizeof(page_directory_));
        resolver_.set_cr3(convert_logical_to_physical_address((uintptr_t)&page_directory_));
        mmu_.set_cr3(resolver_.get_cr3());
        bootstrapper_.prepare_address_space(mmu_);
    }
};

TEST_CASE("i386::PhysicalMemoryMap::map_page -- basic example", "[i386]")
{
    // Showcase the most basic use of map_page. This maps a single page in the
    // kernel virtual memory region to a single page frame at the corresponding
    // physical address.

    // Setup
    TestPagingContext context;
    i386::PhysicalMemoryMap phys_map;
    phys_map.load(context.mmu_);

    // Action
    phys_map.map_page(convert_logical_to_physical_address(KERNEL_VIRTUAL_START_ADDR),
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE | phys_map.GLOBAL);

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
    REQUIRE(pte->is_global() == true);
    REQUIRE(pte->get_address() == convert_logical_to_physical_address(KERNEL_VIRTUAL_START_ADDR));
}

TEST_CASE("i386::PhysicalMemoryMap::set_readonly -- zero size region", "[i386]")
{
    // Setting a zero-size region as read-only is effectively a no-op.

    // Setup
    TestPagingContext context;
    i386::PhysicalMemoryMap phys_map;
    phys_map.load(context.mmu_);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(KERNEL_VIRTUAL_START_ADDR, KERNEL_VIRTUAL_START_ADDR);

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}

TEST_CASE("i386::PhysicalMemoryMap::set_readonly -- one-byte region region", "[i386]")
{
    // Memory access permission are set on a per-page basis. So, setting a one
    // byte long region as read-only changes the entire associated page.

    // Setup
    TestPagingContext context;
    i386::PhysicalMemoryMap phys_map;
    phys_map.load(context.mmu_);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(KERNEL_VIRTUAL_START_ADDR, KERNEL_VIRTUAL_START_ADDR+1);

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == false);

    pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}

TEST_CASE("i386::PhysicalMemoryMap::set_readonly -- one-page region region", "[i386]")
{
    // If we set a PAGE_SIZE region as read-only then the associated page is
    // marked as read-only. The next page is not, because the end of the range
    // is exclusive, e.g., defined as [BEGIN, END)

    // Setup
    TestPagingContext context;
    i386::PhysicalMemoryMap phys_map;
    phys_map.load(context.mmu_);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(KERNEL_VIRTUAL_START_ADDR, KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE);

    // Test
    i386::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == false);

    pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}
