#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

#include "logical_addressing.hpp"
#include "kernel_image_info.hpp" // for PAGE_SIZE

#define assert_is_page_aligned(address) assert(((uintptr_t)(address) & (PAGE_SIZE-1)) == 0)

constexpr unsigned PHYS_MAP_PRESENT = (1 << 0);
constexpr unsigned PHYS_MAP_WRITABLE = (1 << 1);

struct page_table;

// Represents a "Page Directory Entry" on Intel x86 CPUs.
struct page_directory_entry {
    static constexpr unsigned PDE_FLAGS_MASK = 0xFFF;

    uint32_t pde;

    // Gets the physical address of the page table associated with this PDE.
    uintptr_t get_page_table_physical_address() const;

    // Sets the physical address of the page table associated with this PDE.
    void set_page_table_physical_address(uintptr_t physical_address);

    // Sets the page table associated with this PDE.
    // This method assumes the page table has a kernel logical address.
    void set_page_table(page_table * const pt);

    // Sets the flag bits for the PDE.
    void set_flags(unsigned flags);

    // Returns true if the PRESENT bit is set.
    bool is_present() const;

    // Return a reference to the page table associated with this PDE.
    // This method assumes the page table has a kernel logical address.
    page_table& get_page_table();
    const page_table& get_page_table() const;
};
static_assert(sizeof(page_directory_entry) == sizeof(uint32_t),
              "A page directory entry on x86 must be four bytes.");

// Represents a "Page Table Entry" on Intel x86 CPUs.
struct page_table_entry {
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
static_assert(sizeof(page_table_entry) == sizeof(uint32_t),
              "A page table entry on x86 must be four bytes.");

// Represents a "Page Table" on Intel x86 CPUs.
struct page_table {
    static constexpr size_t NUMBER_OF_PAGE_TABLE_ENTRIES = 1024;

    page_table_entry entries[NUMBER_OF_PAGE_TABLE_ENTRIES];
    
    // Return the page table entry associated with the specified virtual
    // address.
    size_t index_for_virtual_address(uintptr_t virtual_address) const;
};
static_assert(sizeof(page_table) == PAGE_SIZE,
              "A page table on x86 must be 4096 bytes.");

// Represents a "Page Directory" on Intel x86 CPUs.
struct page_directory {
    static constexpr size_t NUMBER_OF_PAGE_DIRECTORY_ENTRIES = 1024;

    page_directory_entry entries[NUMBER_OF_PAGE_DIRECTORY_ENTRIES];

    // Return the page directory entry associated with the specified virtual
    // address.
    size_t index_for_virtual_address(uintptr_t virtual_address) const;
    
    // Return the PDE associated with the specified virtual address.
    page_directory_entry& get_pde(uintptr_t virtual_address);
    const page_directory_entry& get_pde(uintptr_t virtual_address) const;

    // Return the PTE associated with the specified virtual address.
    page_table_entry& get_pte(uintptr_t virtual_address);
    const page_table_entry& get_pte(uintptr_t virtual_address) const;

    // Return true if the specified virtual address is mapped in the page
    // directory and corresponding page table, and false otherwise.
    bool is_address_mapped(uintptr_t virtual_address) const;

    // Map the specified physical page to the virtual page.
    // Use `flags' to control the permissions.
    void map_page(uintptr_t physical_address,
                  uintptr_t virtual_address,
                  unsigned flags);
};
static_assert(sizeof(page_directory) == PAGE_SIZE,
              "A page directory on x86 must be 4096 bytes.");

// Gets the 4KB page associated with the given address.
uintptr_t get_page_for_address(uintptr_t address);

// Invalidate the TLB entry for the specified virtual address.
void invlpg(void* virtual_address);

// Gets the page directory currently bound to the MMU.
page_directory& get_current_page_directory();
const page_directory& get_current_page_directory_const();
