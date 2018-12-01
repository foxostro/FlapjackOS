#include <platform/i386/page_table_entry.hpp>
#include <platform/i386/protection_flags.hpp>
#include <page_size.hpp>

namespace i386 {

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
        pte = pte | PAGING_WRITABLE;
    } else {
        pte = pte & ~PAGING_WRITABLE;
    }
}

bool PageTableEntry::is_present() const
{
    return (pte & PAGING_PRESENT) == PAGING_PRESENT;
}

} // namespace i386