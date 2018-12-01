#include <platform/i386/physical_memory_map.hpp>
#include <platform/i386/mmu.hpp> // for invlpg() and get_current_page_directory()
#include <cassert>

namespace i386 {

constexpr unsigned PhysicalMemoryMap::PRESENT;
constexpr unsigned PhysicalMemoryMap::WRITABLE;

PhysicalMemoryMap::PhysicalMemoryMap() = default;

void PhysicalMemoryMap::map_page(uintptr_t physical_address,
                                 uintptr_t virtual_address,
                                 ProtectionFlags flags)
{
    unsigned pd_flags = translate_protection_flags(flags);
    PageDirectory& page_directory = get_page_directory();
    page_directory.map_page(physical_address, virtual_address, pd_flags);
    invalidate_page(virtual_address);
}

void PhysicalMemoryMap::set_readonly(uintptr_t begin, uintptr_t end)
{
    PageDirectory& page_directory = get_page_directory();
    for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
        page_directory.get_pte(page).set_writable(false);
    }
    invalidate_pages(begin, end);
}

void PhysicalMemoryMap::invalidate_pages(uintptr_t begin, uintptr_t end)
{
    for (uintptr_t page = begin; page < end; page += PAGE_SIZE) {
        invlpg((void*)page);
    }
}

void PhysicalMemoryMap::invalidate_page(uintptr_t page)
{
    invlpg((void*)page);
}

PageDirectory& PhysicalMemoryMap::get_page_directory()
{
    return get_current_page_directory();
}

unsigned PhysicalMemoryMap::translate_protection_flags(ProtectionFlags flags)
{
    unsigned result = 0;
    if ((flags & PRESENT) == PRESENT) {
        result = result | PAGING_PRESENT;
    }
    if ((flags & WRITABLE) == WRITABLE) {
        result = result | PAGING_WRITABLE;
    }
    return result;
}

} // namespace i386