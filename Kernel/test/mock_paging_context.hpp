#ifndef FLAPJACKOS_KERNEL_TEST_MOCK_PAGING_CONTEXT_HPP
#define FLAPJACKOS_KERNEL_TEST_MOCK_PAGING_CONTEXT_HPP

#include <cstdint>
#include "mock_memory_management_unit.hpp"
#include <concrete_page_frame_allocator.hpp>

#if defined(__i386__)

#include <platform/i386/kernel_address_space_bootstrapper.hpp>

class MockPagingContext
{
public:
    i386::PageDirectory page_directory_;
    MockMemoryManagementUnit mmu_;
    i386::PagingResolver resolver_;
    i386::KernelAddressSpaceBootstrapper bootstrapper_;
    ConcretePageFrameAllocator page_frame_allocator_;

    MockPagingContext()
     : resolver_(mmu_),
       bootstrapper_(mmu_)
    {
        memset(&page_directory_, 0, sizeof(page_directory_));
        uintptr_t cr3 = mmu_.convert_logical_to_physical_address((uintptr_t)&page_directory_);
        resolver_.set_cr3(cr3);
        mmu_.set_cr3(cr3);
        bootstrapper_.prepare_address_space();
    }
};

#elif defined(__x86_64__)

#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>

class MockPagingContext
{
public:
    x86_64::PageMapLevelFour pml4_;
    x86_64::PageDirectoryPointerTable pdpt_;
    x86_64::PageDirectory page_directory_;
    MockMemoryManagementUnit mmu_;
    x86_64::PagingResolver resolver_;
    x86_64::KernelAddressSpaceBootstrapper bootstrapper_;
    ConcretePageFrameAllocator page_frame_allocator_;

    MockPagingContext()
     : resolver_(mmu_),
       bootstrapper_(mmu_)
    {
        memset(&pml4_, 0, sizeof(pml4_));
        memset(&pdpt_, 0, sizeof(pdpt_));
        memset(&page_directory_, 0, sizeof(page_directory_));
        uintptr_t cr3 = mmu_.convert_logical_to_physical_address((uintptr_t)&pml4_);
        resolver_.set_cr3(cr3);
        mmu_.set_cr3(cr3);
        resolver_.get_page_map_level_four_entry(&pml4_, mmu_.get_kernel_virtual_start_address())->set_address(mmu_.convert_logical_to_physical_address((uintptr_t)&pdpt_));
        resolver_.get_page_directory_pointer_table_entry(&pdpt_, mmu_.get_kernel_virtual_start_address())->set_address(mmu_.convert_logical_to_physical_address((uintptr_t)&page_directory_));
        bootstrapper_.prepare_address_space();
    }
};

#endif

#endif // FLAPJACKOS_KERNEL_TEST_MOCK_PAGING_CONTEXT_HPP
