#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGING_RESOLVER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGING_RESOLVER_HPP

#include "page_table.hpp"
#include "page_directory.hpp"
#include <cstdint>
#include <logical_addressing.hpp> // for convert_physical_to_logical_address()

namespace i386 {

// Walks the paging structures.
// The idea is to extract this stuff to make it easy to implement a mock for
// testing. Imagine implementing a version of this class to run on an x86_64
// host in order to test the i386 paging structures, or vice-versa.
class PagingResolver {
public:
    using PageDirectory = i386::PageDirectory;
    using PageTable = i386::PageTable;

    PagingResolver() : cr3_(0) {}

    uint32_t get_cr3() const
    {
        return cr3_;
    }

    void set_cr3(uint32_t value)
    {
        cr3_ = value;
    }

    template<typename Function>
    void enumerate_page_directory_entries(PageDirectory& pd,
                                          uintptr_t linear_address,
                                          size_t length,
                                          Function&& fn)
    {
        constexpr uintptr_t STEP = 0x400000;
        for (size_t remaining_length = length;
             remaining_length > 0;
             remaining_length -= STEP) {
            PageDirectoryEntry* pde = get_page_directory_entry(&pd, linear_address);
            assert(pde);
            fn(*pde);
            linear_address += STEP;
        }
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
        PageDirectory* pd = get_page_directory();
        if (!pd) {
            return nullptr;
        }

        PageTable* pt = get_page_table(pd, linear_address);
        return pt;
    }

    PageDirectory* get_page_directory(uintptr_t)
    { 
        return get_page_directory();
    }

    PageDirectory* get_page_directory()
    { 
        uintptr_t address = (uintptr_t)cr3_;
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

    // Given a linear address, return the corresponding PDE index.
    // A portion of the linear address determines the index of the corresponding
    // entry in the Page Directory.
    // See the Intel manual, Volume 3a, Section 4.3, Figure 4-2 for details.
    size_t get_page_directory_index(uintptr_t linear_address)
    {
        constexpr uint32_t PDE_INDEX_SHIFT = 22;
        size_t index = linear_address >> PDE_INDEX_SHIFT;
        return index;
    }

    // Given a linear address, return the corresponding PTE index.
    // A portion of the linear address determines the index of the corresponding
    // entry in the Page Table.
    // See the Intel manual, Volume 3a, Section 4.3, Figure 4-2 for details.
    size_t get_page_table_index(uintptr_t linear_address)
    {
        constexpr uint32_t PTE_INDEX_SHIFT = 12;
        constexpr uint32_t PTE_INDEX_MASK = 0x03FF;
        size_t index = linear_address >> PTE_INDEX_SHIFT & PTE_INDEX_MASK;
        return index;
    }

private:
    uint32_t cr3_;
};

static_assert(sizeof(uint32_t) == sizeof(uintptr_t),
              "By it's nature, the i386::PagingResolver must be able to map "
              "between a pointer and a 32-bit unsigned int.");

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_PAGING_RESOLVER_HPP