#include "catch.hpp"
#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <platform/x86_64/physical_memory_map.hpp>
#include "mock_memory_management_unit.hpp"

class TestPagingContext
{
public:
    x86_64::PageMapLevelFour pml4_;
    x86_64::PageDirectoryPointerTable pdpt_;
    x86_64::PageDirectory page_directory_;
    MockMemoryManagementUnit mmu_;
    x86_64::PagingResolver<MockMemoryManagementUnit> resolver_;
    x86_64::KernelAddressSpaceBootstrapper<MockMemoryManagementUnit> bootstrapper_;

    TestPagingContext()
     : resolver_(mmu_),
       bootstrapper_(mmu_)
    {
        memset(&pml4_, 0, sizeof(pml4_));
        memset(&pdpt_, 0, sizeof(pdpt_));
        memset(&page_directory_, 0, sizeof(page_directory_));
        uintptr_t cr3 = mmu_.convert_logical_to_physical_address((uintptr_t)&pml4_);
        resolver_.set_cr3(cr3);
        mmu_.set_cr3(cr3);
        resolver_.get_page_map_level_four_entry(&pml4_, KERNEL_VIRTUAL_START_ADDR)->set_address(mmu_.convert_logical_to_physical_address((uintptr_t)&pdpt_));
        resolver_.get_page_directory_pointer_table_entry(&pdpt_, KERNEL_VIRTUAL_START_ADDR)->set_address(mmu_.convert_logical_to_physical_address((uintptr_t)&page_directory_));
        bootstrapper_.prepare_address_space();
    }
};

TEST_CASE("x86_64::PhysicalMemoryMap::map_page -- basic example", "[x86_64]")
{
    // Showcase the most basic use of map_page. This maps a single page in the
    // kernel virtual memory region to a single page frame at the corresponding
    // physical address.

    // Setup
    TestPagingContext context;
    x86_64::PhysicalMemoryMap<MockMemoryManagementUnit> phys_map(context.mmu_);
    phys_map.reload();

    // Action
    phys_map.map_page(context.mmu_.convert_logical_to_physical_address(KERNEL_VIRTUAL_START_ADDR),
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE | phys_map.GLOBAL);

    // Test
    x86_64::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
    REQUIRE(pte->is_global() == true);
    REQUIRE(pte->get_address() == context.mmu_.convert_logical_to_physical_address(KERNEL_VIRTUAL_START_ADDR));
}

TEST_CASE("x86_64::PhysicalMemoryMap::set_readonly -- zero size region", "[x86_64]")
{
    // Setting a zero-size region as read-only is effectively a no-op.

    // Setup
    TestPagingContext context;
    x86_64::PhysicalMemoryMap<MockMemoryManagementUnit> phys_map(context.mmu_);
    phys_map.reload();
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(KERNEL_VIRTUAL_START_ADDR, KERNEL_VIRTUAL_START_ADDR);

    // Test
    x86_64::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}

TEST_CASE("x86_64::PhysicalMemoryMap::set_readonly -- one-byte region region", "[x86_64]")
{
    // Memory access permission are set on a per-page basis. So, setting a one
    // byte long region as read-only changes the entire associated page.

    // Setup
    TestPagingContext context;
    x86_64::PhysicalMemoryMap<MockMemoryManagementUnit> phys_map(context.mmu_);
    phys_map.reload();
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(KERNEL_VIRTUAL_START_ADDR, KERNEL_VIRTUAL_START_ADDR+1);

    // Test
    x86_64::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == false);

    pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}

TEST_CASE("x86_64::PhysicalMemoryMap::set_readonly -- one-page region region", "[x86_64]")
{
    // If we set a PAGE_SIZE region as read-only then the associated page is
    // marked as read-only. The next page is not, because the end of the range
    // is exclusive, e.g., defined as [BEGIN, END)

    // Setup
    TestPagingContext context;
    x86_64::PhysicalMemoryMap<MockMemoryManagementUnit> phys_map(context.mmu_);
    phys_map.reload();
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR,
                      KERNEL_VIRTUAL_START_ADDR,
                      phys_map.WRITABLE);
    phys_map.map_page(KERNEL_PHYSICAL_LOAD_ADDR+PAGE_SIZE,
                      KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE,
                      phys_map.WRITABLE);

    // Action
    phys_map.set_readonly(KERNEL_VIRTUAL_START_ADDR, KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE);

    // Test
    x86_64::PageTableEntry* pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == false);

    pte = context.resolver_.get_page_table_entry(KERNEL_VIRTUAL_START_ADDR+PAGE_SIZE);
    REQUIRE(pte != nullptr);
    REQUIRE(pte->is_present() == true);
    REQUIRE(pte->is_readwrite() == true);
}
