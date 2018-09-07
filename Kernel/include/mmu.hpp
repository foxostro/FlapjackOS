#ifndef FLAPJACKOS_KERNEL_INCLUDE_MMU_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_MMU_HPP

#include <cstddef>
#include <cstdint>

#include "logical_addressing.hpp"
#include "page_size.hpp"

constexpr unsigned PHYS_MAP_PRESENT = (1 << 0);
constexpr unsigned PHYS_MAP_WRITABLE = (1 << 1);

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

// Represents a "Page Table" on Intel x86 CPUs.
struct PageTable {
    static constexpr size_t NUMBER_OF_PAGE_TABLE_ENTRIES = 1024;

    PageTableEntry entries[NUMBER_OF_PAGE_TABLE_ENTRIES];
    
    // Return the page table entry associated with the specified virtual
    // address.
    size_t index_for_virtual_address(uintptr_t virtual_address) const;
};
static_assert(sizeof(PageTable) == PAGE_SIZE,
              "A page table on x86 must be 4096 bytes.");

// Represents a "Page Directory" on Intel x86 CPUs.
struct PageDirectory {
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

// Gets the 4KB page associated with the given address.
uintptr_t get_page_for_address(uintptr_t address);

// Invalidate the TLB entry for the specified virtual address.
void invlpg(void* virtual_address);

// Gets the page directory currently bound to the MMU.
PageDirectory& get_current_page_directory();
const PageDirectory& get_current_page_directory_const();

#endif // FLAPJACKOS_KERNEL_INCLUDE_MMU_HPP
