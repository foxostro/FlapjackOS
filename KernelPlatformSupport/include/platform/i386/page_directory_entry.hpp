#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_ENTRY_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_ENTRY_HPP

#include <cstdint>

namespace i386 {

struct PageTable;

// Represents a "Page Directory Entry" on Intel x86 CPUs.
struct PageDirectoryEntry {
    static constexpr unsigned PDE_FLAGS_MASK = 0xFFF;

    uint32_t pde;

    // Gets the physical address of the page table associated with this PDE.
    uintptr_t get_page_table_physical_address() const;

    // Sets the physical address of the page table associated with this PDE.
    void set_page_table_physical_address(uintptr_t physical_address);

    // Sets the page table associated with this PDE.
    // This method assumes the page table has a kernel logical address.
    void set_page_table(PageTable * const pt);

    // Sets the flag bits for the PDE.
    void set_flags(unsigned flags);

    // Returns true if the PRESENT bit is set.
    bool is_present() const;

    // Return a reference to the page table associated with this PDE.
    // This method assumes the page table has a kernel logical address.
    PageTable& get_page_table();
    const PageTable& get_page_table() const;
};
static_assert(sizeof(PageDirectoryEntry) == sizeof(uint32_t),
              "A page directory entry on x86 must be four bytes.");

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_DIRECTORY_ENTRY_HPP