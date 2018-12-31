#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PHYSICAL_MEMORY_MAP_HPP

#include <cstdint>
#include <type_traits>
#include "page_size.hpp"

namespace i386 {

// Provides a platform-agnostic interface to the hardware MMU memory map.
// It's physical because it's hardware.
template<typename PagingResolver>
class GenericPhysicalMemoryMap {
public:
    using PageTableEntry = typename std::remove_reference<decltype(PagingResolver::PageTable::entries[0])>::type;
    using MMU = typename PagingResolver::MMU;
    using ProtectionFlags = unsigned;

    static constexpr unsigned PRESENT = (1 << 0);
    static constexpr unsigned WRITABLE = (1 << 1);
    static constexpr unsigned GLOBAL = (1 << 2);
    static constexpr unsigned SUPERVISOR = (1 << 3);

    GenericPhysicalMemoryMap(MMU &mmu)
     : resolver_(mmu),
       mmu_(mmu)
    {}

    // Point the physical memory map at the paging structures active on the MMU.
    // So, whatever the MMU is using right now is what PhysicalMemoryMap will
    // now act upon.
    void reload()
    {
        resolver_.set_cr3(mmu_.get_cr3());
    }

    // Allocate memory for page tables and insert them into the page directory.
    virtual void populate_page_tables(uintptr_t begin, size_t length) = 0;

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    void map_page(uintptr_t physical_address,
                  uintptr_t linear_address,
                  ProtectionFlags flags)
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
    void set_readonly(uintptr_t begin, uintptr_t end)
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            PageTableEntry* pte = resolver_.get_page_table_entry(page);
            assert(pte);
            pte->set_readwrite(false);
        }
        invalidate_pages(begin, end);
    }

    // Invalidates the hardware MMU cache for the specified virtual addresses.
    void invalidate_pages(uintptr_t begin, uintptr_t end)
    {
        for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
            invlpg(page);
        }
    }

    // Invalidates the hardware MMU cache for the specified virtual address.
    void invalidate_page(uintptr_t linear_address)
    {
        invlpg(linear_address);
    }

protected:
    PagingResolver resolver_;
    MMU &mmu_;

private:
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

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_GENERIC_PHYSICAL_MEMORY_MAP_HPP