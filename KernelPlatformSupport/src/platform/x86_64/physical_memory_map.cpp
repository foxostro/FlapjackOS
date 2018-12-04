#include <platform/x86_64/physical_memory_map.hpp>
#include <platform/x86_64/page_map_level_four.hpp>
#include <platform/x86_64/page_directory_pointer_table.hpp>
#include <platform/x86_64/page_directory.hpp>
#include <platform/x86_64/page_table.hpp>
#include <cassert>

namespace x86_64 {

inline void invlpg(uintptr_t virtual_address)
{
#ifdef TESTING
    (void)virtual_address;
#else
    asm volatile("invlpg (%0)" : : "b"(virtual_address) : "memory");
#endif
}

PhysicalMemoryMap::PhysicalMemoryMap()
{
    assert(!"stub");
}

void PhysicalMemoryMap::map_page(uintptr_t,
                                 uintptr_t,
                                 ProtectionFlags)
{
    assert(!"stub");
}

void PhysicalMemoryMap::set_readonly(uintptr_t, uintptr_t)
{
    assert(!"stub");
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

} // namespace x86_64