#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGING_RESOLVER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGING_RESOLVER_HPP

#include "page_table.hpp"
#include "page_directory.hpp"
#include <hardware_memory_management_unit.hpp>
#include <cstdint>
#include <limits>

namespace i386 {

// Walks the paging structures and understands the relationships between them.
class PagingResolver {
public:
    using PageDirectory = i386::PageDirectory;
    using PageTable = i386::PageTable;

    PagingResolver(HardwareMemoryManagementUnit &mmu) : cr3_(0), mmu_(mmu) {}

    uint32_t get_cr3() const
    {
        return cr3_;
    }

    void set_cr3(uint32_t value)
    {
        cr3_ = value;
    }

    // Enumerate PDEs governing the specified memory range:
    //     [linear_address, linear_address+length)
    // For each PDE, execute fn(pde) where pde is a "PageDirectoryEntry*".
    // Each page table governs 4MB of memory.
    template<typename Function>
    void enumerate_page_directory_entries(uintptr_t linear_address,
                                          size_t length,
                                          Function&& fn)
    {
        assert(length < std::numeric_limits<int>::max());

        // Each page table governs 4MB of memory.
        constexpr int STEP = 0x400000;
        
        for (int remaining_length = static_cast<int>(length);
             remaining_length > 0;
             remaining_length -= STEP) {
            PageDirectoryEntry* pde = get_page_directory_entry(get_page_directory(), linear_address);
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
        PageDirectory* pd = (PageDirectory*)mmu_.convert_physical_to_logical_address(cr3_);
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

#if 0
        uint64_t address = entry->get_address();
#else
        // Defeat the optimizer. Without this memcpy() the optimizer will
        // remove the branch "if (address == 0x0)". I have no idea why.
        uint64_t tmp = entry->get_address();
        uint64_t address = 0;
        memcpy(&address, &tmp, sizeof(tmp));
#endif

        if (address == 0x0) {
            return nullptr;
        }
        uintptr_t pt_addr = mmu_.convert_physical_to_logical_address(address);
        PageTable* pt = reinterpret_cast<PageTable*>(pt_addr);
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
    HardwareMemoryManagementUnit &mmu_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGING_RESOLVER_HPP