#include <platform/i386/page_directory_entry.hpp>
#include <platform/i386/protection_flags.hpp>
#include <logical_addressing.hpp>
#include <page_size.hpp>

namespace i386 {

uintptr_t PageDirectoryEntry::get_page_table_physical_address() const
{
    return pde & ~PDE_FLAGS_MASK;
}

void PageDirectoryEntry::set_page_table_physical_address(uintptr_t physical_address)
{
    assert_is_page_aligned(physical_address);
    pde = physical_address | (pde & PDE_FLAGS_MASK) | PAGING_PRESENT;
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
    return (pde & PAGING_PRESENT) == PAGING_PRESENT;
}

PageTable& PageDirectoryEntry::get_page_table()
{
    return *(PageTable*)convert_physical_to_logical_address(get_page_table_physical_address());
}

const PageTable& PageDirectoryEntry::get_page_table() const
{
    return *(const PageTable*)convert_physical_to_logical_address(get_page_table_physical_address());
}

} // namespace i386