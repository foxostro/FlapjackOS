#include <platform/i386/page_directory.hpp>
#include <platform/i386/mmu.hpp> // for invlpg()

namespace i386 {

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
    pde.set_flags(flags | PAGING_PRESENT);

    PageTableEntry &pte = get_pte(virtual_address);
    pte.set_page_frame(physical_address);
    pte.set_flags(flags | PAGING_PRESENT);

    invlpg((void*)virtual_address);
}

} // namespace i386