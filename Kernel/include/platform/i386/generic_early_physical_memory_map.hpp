#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_EARLY_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_EARLY_PHYSICAL_MEMORY_MAP_HPP

#include <cstdint>
#include <type_traits>
#include "page_size.hpp"
#include <hardware_memory_management_unit.hpp>
#include <physical_memory_map.hpp>

namespace i386 {

// Provides a platform-agnostic interface to the hardware MMU memory map.
// It's physical because it's hardware.
template<typename PagingResolver>
class GenericEarlyPhysicalMemoryMap : public ::PhysicalMemoryMap {
public:
    using PageTableEntry = typename std::remove_reference<decltype(PagingResolver::PageTable::entries[0])>::type;

    GenericEarlyPhysicalMemoryMap(HardwareMemoryManagementUnit &mmu)
     : resolver_(mmu),
       mmu_(mmu)
    {}

    // Point the physical memory map at the paging structures active on the MMU.
    // So, whatever the MMU is using right now is what EarlyPhysicalMemoryMap
    // will now act upon.
    void reload() override
    {
        resolver_.set_cr3(mmu_.get_cr3());
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
        invalidate_page(linear_address);
    }

    // Mark the specified range of virtual pages as read-only.
    void set_readonly(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            PageTableEntry* pte = resolver_.get_page_table_entry(page);
            assert(pte);
            pte->set_readwrite(false);
        }
        invalidate_pages(begin, end);
    }

    // Invalidates the hardware MMU cache for the specified virtual addresses.
    void invalidate_pages(uintptr_t begin, uintptr_t end) override
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            invlpg(page);
        }
    }

    // Invalidates the hardware MMU cache for the specified virtual address.
    void invalidate_page(uintptr_t linear_address) override
    {
        invlpg(linear_address);
    }

protected:
    PagingResolver resolver_;
    HardwareMemoryManagementUnit &mmu_;

private:
    // AFOX_TODO: Perhaps invlpg() should move to HardwareMemoryManagementUnit?
    inline void invlpg(uintptr_t linear_address)
    {
    #ifdef TESTING
        (void)linear_address;
    #else
        asm volatile("invlpg (%0)" : : "b"(linear_address) : "memory");
    #endif
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_EARLY_PHYSICAL_MEMORY_MAP_HPP