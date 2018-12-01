#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_HPP

#include <cstdint>
#include <page_size.hpp>
#include "page_table_entry.hpp"
#include "page_directory_entry.hpp"

namespace i386 {

// Represents a "Page Directory" on Intel x86 CPUs.
struct alignas(PAGE_SIZE) PageDirectory {
    static constexpr size_t NUMBER_OF_PAGE_DIRECTORY_ENTRIES = 1024;

    PageDirectoryEntry entries[NUMBER_OF_PAGE_DIRECTORY_ENTRIES];

    // Return the page directory entry associated with the specified virtual
    // address.
    size_t index_for_virtual_address(uintptr_t virtual_address) const;
    
    // Return the PDE associated with the specified virtual address.
    PageDirectoryEntry& get_pde(uintptr_t virtual_address);
    const PageDirectoryEntry& get_pde(uintptr_t virtual_address) const;

    // Return the PTE associated with the specified virtual address.
    PageTableEntry& get_pte(uintptr_t virtual_address);
    const PageTableEntry& get_pte(uintptr_t virtual_address) const;

    // Return true if the specified virtual address is mapped in the page
    // directory and corresponding page table, and false otherwise.
    bool is_address_mapped(uintptr_t virtual_address) const;

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    void map_page(uintptr_t physical_address,
                  uintptr_t virtual_address,
                  unsigned flags);
};
static_assert(sizeof(PageDirectory) == PAGE_SIZE,
              "A page directory on x86 must be 4096 bytes.");

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_HPP