#include <platform/i386/physical_memory_map.hpp>
#include <cassert>

namespace i386 {

inline void invlpg(uintptr_t linear_address)
{
#ifdef TESTING
    (void)linear_address;
#else
    asm volatile("invlpg (%0)" : : "b"(linear_address) : "memory");
#endif
}

PhysicalMemoryMap::PhysicalMemoryMap() = default;

void PhysicalMemoryMap::map_page(uintptr_t physical_address,
                                 uintptr_t linear_address,
                                 ProtectionFlags flags)
{
    PageTableEntry* pte = resolver_.get_page_table_entry(linear_address);
    assert(pte);
    pte->set_address(physical_address);
    pte->set_present(physical_address != 0);
    pte->set_readwrite((flags & WRITABLE) == WRITABLE);
    invalidate_page(linear_address);
}

void PhysicalMemoryMap::set_readonly(uintptr_t begin, uintptr_t end)
{
    for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
        PageTableEntry* pte = resolver_.get_page_table_entry(page);
        assert(pte);
        pte->set_readwrite(false);
    }
    invalidate_pages(begin, end);
}

void PhysicalMemoryMap::invalidate_pages(uintptr_t begin, uintptr_t end)
{
    for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
        invlpg(page);
    }
}

void PhysicalMemoryMap::invalidate_page(uintptr_t page)
{
    invlpg(page);
}

} // namespace i386