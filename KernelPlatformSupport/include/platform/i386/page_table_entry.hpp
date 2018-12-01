#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_ENTRY_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_ENTRY_HPP

#include <cstdint>

namespace i386 {

// Represents a "Page Table Entry" on Intel x86 CPUs.
struct PageTableEntry {
    static constexpr unsigned PTE_FLAGS_MASK = 0xFFF;

    uint32_t pte;

    // Gets the page frame associated with the PTE.
    uintptr_t get_page_frame() const;

    // Sets the page frame associated with the PTE.
    void set_page_frame(uintptr_t physical_address);

    // Sets the flag bits for the PTE.
    void set_flags(unsigned flags);

    // Sets the WRITABLE bit to true/false according to parameter `writable'.
    void set_writable(bool writable);

    // Returns true if the PRESENT bit is set.
    bool is_present() const;
};
static_assert(sizeof(PageTableEntry) == sizeof(uint32_t),
              "A page table entry on x86 must be four bytes.");

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGE_TABLE_ENTRY_HPP