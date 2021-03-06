#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP

#include <cstdint>
#include <type_traits>
#include "page_size.hpp"
#include <hardware_memory_management_unit.hpp>
#include <physical_memory_map.hpp>

namespace i386 {

// Provides a platform-agnostic interface to the hardware MMU memory map.
// It's physical because it's hardware.
// This version is "unmanaged" in the sense that it provides no mechanisms for
// tracking ownership of underlying paging objects or page frames, which is
// perfectly acceptable before the heap is available.
template<typename PagingResolver>
class GenericUnmanagedPhysicalMemoryMap : public ::PhysicalMemoryMap {
public:
    using PageTableEntry = typename std::remove_reference<decltype(PagingResolver::PageTable::entries[0])>::type;

    GenericUnmanagedPhysicalMemoryMap(HardwareMemoryManagementUnit &mmu)
     : resolver_(mmu),
       mmu_(mmu)
    {}
    
    // Map virtual memory with the given protection.
    // Assumes a kernel logical mapping for memory in this region.
    void map_pages(uintptr_t begin,
                   uintptr_t end,
                   ProtectionFlags flags) override
    {
        assert(begin < end);
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            map_page(mmu_.convert_logical_to_physical_address(page),
                     page,
                     flags);
        }
    }

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    void map_page(uintptr_t physical_address,
                  uintptr_t linear_address,
                  ProtectionFlags flags) override
    {
        PageTableEntry* pte = resolver_.get_page_table_entry(linear_address);
        assert(pte);
        pte->set_address(physical_address);
        pte->set_present(physical_address != 0);
        pte->set_readwrite((flags & WRITABLE) == WRITABLE);
        pte->set_global((flags & GLOBAL) == GLOBAL);
        pte->set_supervisor((flags & SUPERVISOR) == SUPERVISOR);
        mmu_.invalidate_page(linear_address);
    }

    // Mark the specified range of virtual pages as read-only.
    void set_readonly(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            PageTableEntry* pte = resolver_.get_page_table_entry(page);
            assert(pte);
            pte->set_readwrite(false);
        }
        mmu_.invalidate_pages(begin, end);
    }

protected:
    PagingResolver resolver_;
    HardwareMemoryManagementUnit &mmu_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP