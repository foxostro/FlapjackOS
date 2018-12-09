#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGING_RESOLVER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGING_RESOLVER_HPP

#include "page_table.hpp"
#include "page_directory.hpp"
#include "page_directory_pointer_table.hpp"
#include "page_map_level_four.hpp"
#include <cstdint>
#include <logical_addressing.hpp> // for convert_physical_to_logical_address()

namespace x86_64 {

// Walks the paging structures.
// The idea is to extract this stuff to make it easy to implement a mock for
// testing. Imagine implementing a version of this class to run on an x86_64
// host in order to test the i386 paging structures, or vice-versa.
class PagingResolver {
public:
    PagingResolver() : cr3_(0) {}

    uint64_t get_cr3() const
    {
        return cr3_;
    }

    void set_cr3(uint64_t value)
    {
        cr3_ = value;
    }

    PageTableEntry* get_page_table_entry(uintptr_t linear_address)
    {
        PageTable* pt = get_page_table(linear_address);
        if (!pt) {
            return nullptr;
        }
        PageTableEntry* pte = get_page_table_entry(pt, linear_address);
        return pte;
    }

    PageTable* get_page_table(uintptr_t linear_address)
    {
        PageMapLevelFour* pml4 = get_page_map_level_four();
        if (!pml4) {
            return nullptr;
        }

        PageDirectoryPointerTable* pdpt = get_page_directory_pointer_table(pml4, linear_address);
        if (!pdpt) {
            return nullptr;
        }

        PageDirectory* pd = get_page_directory(pdpt, linear_address);
        if (!pd) {
            return nullptr;
        }

        PageTable* pt = get_page_table(pd, linear_address);
        return pt;
    }

    PageDirectory* get_page_directory(uintptr_t linear_address)
    {
        PageMapLevelFour* pml4 = get_page_map_level_four();
        if (!pml4) {
            return nullptr;
        }

        PageDirectoryPointerTable* pdpt = get_page_directory_pointer_table(pml4, linear_address);
        if (!pdpt) {
            return nullptr;
        }

        PageDirectory* pd = get_page_directory(pdpt, linear_address);

        return pd;
    }

    PageMapLevelFour* get_page_map_level_four()
    { 
        uintptr_t address = (uintptr_t)cr3_;
        PageMapLevelFour* pml4 = (PageMapLevelFour*)convert_physical_to_logical_address(address);
        return pml4;
    }

    PageDirectoryPointerTable* get_page_directory_pointer_table(PageMapLevelFour* pml4, uintptr_t linear_address)
    {
        if (!pml4) {
            return nullptr;
        }
        PageMapLevelFourEntry *entry = get_page_map_level_four_entry(pml4, linear_address);
        if (!entry) {
            return nullptr;
        }
        uint64_t address = entry->get_address();
        PageDirectoryPointerTable* pdpt = (PageDirectoryPointerTable*)convert_physical_to_logical_address(address);
        return pdpt;
    }

    PageDirectory* get_page_directory(PageDirectoryPointerTable* pdpt, uintptr_t linear_address)
    {
        if (!pdpt) {
            return nullptr;
        }
        PageDirectoryPointerTableEntry *entry = get_page_directory_pointer_table_entry(pdpt, linear_address);
        if (!entry) {
            return nullptr;
        }
        uint64_t address = entry->get_address();
        PageDirectory* pd = (PageDirectory*)convert_physical_to_logical_address(address);
        return pd;
    }

    PageTable* get_page_table(PageDirectory* pd, uintptr_t linear_address)
    {
        if (!pd) {
            return nullptr;
        }
        PageDirectoryEntry *entry = get_page_directory_entry(pd, linear_address);
        if (!entry) {
            return nullptr;
        }
        uint64_t address = entry->get_address();
        PageTable* pt = (PageTable*)convert_physical_to_logical_address(address);
        return pt;
    }

    PageMapLevelFourEntry* get_page_map_level_four_entry(PageMapLevelFour* pml4, uintptr_t linear_address)
    {
        if (!pml4) {
            return nullptr;
        }
        size_t index = get_page_map_level_four_index(linear_address);
        assert(index < PageMapLevelFour::COUNT);
        return &pml4->entries[index];
    }

    PageDirectoryPointerTableEntry* get_page_directory_pointer_table_entry(PageDirectoryPointerTable* pdpt, uintptr_t linear_address)
    {
        if (!pdpt) {
            return nullptr;
        }
        size_t index = get_page_directory_pointer_table_index(linear_address);
        assert(index < PageDirectoryPointerTable::COUNT);
        return &pdpt->entries[index];
    }

    PageDirectoryEntry* get_page_directory_entry(PageDirectory* pd, uintptr_t linear_address)
    {
        if (!pd) {
            return nullptr;
        }
        size_t index = get_page_directory_index(linear_address);
        assert(index < PageDirectory::COUNT);
        return &pd->entries[index];
    }

    PageTableEntry* get_page_table_entry(PageTable* pt, uintptr_t linear_address)
    {
        if (!pt) {
            return nullptr;
        }
        size_t index = get_page_table_index(linear_address);
        assert(index < PageTable::COUNT);
        return &pt->entries[index];
    }

    // Given a linear address, return the corresponding PML4e index.
    // A portion of the linear address determines the index of the corresponding
    // entry in the PML4 table.
    // See the Intel manual, Volume 3a, Section 4.5, Figure 4-8 for details.
    size_t get_page_map_level_four_index(uintptr_t linear_address)
    {
        constexpr unsigned PML4E_INDEX_SHIFT = 39;
        constexpr unsigned PML4E_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PML4E_INDEX_SHIFT) & PML4E_INDEX_MASK;
        return index;
    }

    // Given a linear address, return the corresponding PDPTe index.
    // A portion of the linear address determines the index of the corresponding
    // entry in the Page Directory Pointer Table.
    // See the Intel manual, Volume 3a, Section 4.5, Figure 4-8 for details.
    size_t get_page_directory_pointer_table_index(uintptr_t linear_address)
    {
        constexpr unsigned PDPTE_INDEX_SHIFT = 30;
        constexpr unsigned PDPTE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PDPTE_INDEX_SHIFT) & PDPTE_INDEX_MASK;
        return index;
    }

    // Given a linear address, return the corresponding PDE index.
    // A portion of the linear address determines the index of the corresponding
    // entry in the Page Directory.
    // See the Intel manual, Volume 3a, Section 4.5, Figure 4-8 for details.
    size_t get_page_directory_index(uintptr_t linear_address)
    {
        constexpr unsigned PDE_INDEX_SHIFT = 21;
        constexpr unsigned PDE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PDE_INDEX_SHIFT) & PDE_INDEX_MASK;
        return index;
    }

    // Given a linear address, return the corresponding PTE index.
    // A portion of the linear address determines the index of the corresponding
    // entry in the Page Table.
    // See the Intel manual, Volume 3a, Section 4.5, Figure 4-8 for details.
    size_t get_page_table_index(uintptr_t linear_address)
    {
        constexpr unsigned PTE_INDEX_SHIFT = 12;
        constexpr unsigned PTE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PTE_INDEX_SHIFT) & PTE_INDEX_MASK;
        return index;
    }

private:
    uint64_t cr3_;
};

static_assert(sizeof(uint64_t) == sizeof(uintptr_t),
              "By it's nature, the x86_64::PagingResolver must be able to map "
              "between a pointer and a 64-bit unsigned int.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGING_RESOLVER_HPP