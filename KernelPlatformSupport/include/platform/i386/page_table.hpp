#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_HPP

#include "page_table_entry.hpp"
#include <page_size.hpp>
#include <cstdint>

namespace i386 {

// Represents a "Page Table" on Intel x86 CPUs.
struct alignas(PAGE_SIZE) PageTable {
    static constexpr size_t NUMBER_OF_PAGE_TABLE_ENTRIES = 1024;

    PageTableEntry entries[NUMBER_OF_PAGE_TABLE_ENTRIES];
    
    // Return the page table entry associated with the specified virtual
    // address.
    size_t index_for_virtual_address(uintptr_t virtual_address) const;
};
static_assert(sizeof(PageTable) == PAGE_SIZE,
              "A page table on x86 must be 4096 bytes.");

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_HPP