#include "mmu.hpp"
#include "creg.h"


uintptr_t PageDirectoryEntry::get_page_table_physical_address() const
{
    return pde & ~PDE_FLAGS_MASK;
}

void PageDirectoryEntry::set_page_table_physical_address(uintptr_t physical_address)
{
    assert_is_page_aligned(physical_address);
    pde = physical_address | (pde & PDE_FLAGS_MASK) | PHYS_MAP_PRESENT;
}

void PageDirectoryEntry::set_page_table(PageTable * const pt)
{
    set_page_table_physical_address(convert_logical_to_physical_address((uintptr_t)pt));
}

void PageDirectoryEntry::set_flags(unsigned flags)
{
    pde = (uint32_t)get_page_table_physical_address() | (flags & PDE_FLAGS_MASK);
}

bool PageDirectoryEntry::is_present() const
{
    return (pde & PHYS_MAP_PRESENT) == PHYS_MAP_PRESENT;
}

PageTable& PageDirectoryEntry::get_page_table()
{
    return *(PageTable*)convert_physical_to_logical_address(get_page_table_physical_address());
}

const PageTable& PageDirectoryEntry::get_page_table() const
{
    return *(const PageTable*)convert_physical_to_logical_address(get_page_table_physical_address());
}





uintptr_t PageTableEntry::get_page_frame() const
{
    return pte & ~PTE_FLAGS_MASK;
}

void PageTableEntry::set_page_frame(uintptr_t physical_address)
{
    assert_is_page_aligned(physical_address);
    pte = physical_address | (pte & PTE_FLAGS_MASK);
}

void PageTableEntry::set_flags(unsigned flags)
{
    pte = (uint32_t)get_page_frame() | (flags & PTE_FLAGS_MASK);
}

void PageTableEntry::set_writable(bool writable)
{
    if (writable) {
        pte = pte & PHYS_MAP_WRITABLE;
    } else {
        pte = pte & ~PHYS_MAP_WRITABLE;
    }
}

bool PageTableEntry::is_present() const
{
    return (pte & PHYS_MAP_PRESENT) == PHYS_MAP_PRESENT;
}





size_t PageDirectory::index_for_virtual_address(uintptr_t virtual_address) const
{
    constexpr uint32_t PDE_INDEX_SHIFT = 22;
    return (uint32_t)virtual_address >> PDE_INDEX_SHIFT;
}

PageDirectoryEntry& PageDirectory::get_pde(uintptr_t virtual_address)
{
    return entries[index_for_virtual_address(virtual_address)];
}

const PageDirectoryEntry& PageDirectory::get_pde(uintptr_t virtual_address) const
{
    return const_cast<PageDirectory*>(this)->get_pde(virtual_address);
}

PageTableEntry& PageDirectory::get_pte(uintptr_t virtual_address)
{
    PageDirectoryEntry& pde = entries[index_for_virtual_address(virtual_address)];
    PageTable& pt = pde.get_page_table();
    size_t page_table_index = pt.index_for_virtual_address(virtual_address);
    PageTableEntry& pte = pt.entries[page_table_index];
    return pte;
}

const PageTableEntry& PageDirectory::get_pte(uintptr_t virtual_address) const
{
    return const_cast<PageDirectory*>(this)->get_pte(virtual_address);
}

bool PageDirectory::is_address_mapped(uintptr_t virtual_address) const
{
    const PageDirectoryEntry& pde = get_pde(virtual_address);
    if (!pde.is_present()) {
        return false;
    }

    const PageTableEntry& pte = get_pte(virtual_address);
    if (!pte.is_present()) {
        return false;
    }

    return true;
}

void PageDirectory::map_page(uintptr_t physical_address,
                              uintptr_t virtual_address,
                              unsigned flags)
{
    assert_is_page_aligned(physical_address);
    assert_is_page_aligned(virtual_address);

    PageDirectoryEntry& pde = get_pde(virtual_address);
    pde.set_flags(flags | PHYS_MAP_PRESENT);

    PageTableEntry &pte = get_pte(virtual_address);
    pte.set_page_frame(physical_address);
    pte.set_flags(flags | PHYS_MAP_PRESENT);

    invlpg((void*)virtual_address);
}





size_t PageTable::index_for_virtual_address(uintptr_t virtual_address) const
{
    constexpr uint32_t PTE_INDEX_SHIFT = 12;
    constexpr uint32_t PTE_INDEX_MASK = 0x03FF;
    return (uint32_t)virtual_address >> PTE_INDEX_SHIFT & PTE_INDEX_MASK;
}





uintptr_t get_page_for_address(uintptr_t address)
{
    return address & ~(PAGE_SIZE-1);
}

void invlpg(void* virtual_address)
{
    asm volatile("invlpg (%0)" : : "b"(virtual_address) : "memory");
}

PageDirectory& get_current_page_directory()
{
    return *(PageDirectory*)convert_physical_to_logical_address(get_cr3());
}

const PageDirectory& get_current_page_directory_const()
{
    return *(const PageDirectory*)convert_physical_to_logical_address(get_cr3());
}
