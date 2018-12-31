#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/generic_physical_memory_map.hpp>
#include "paging_resolver.hpp"
#include "hardware_memory_management_unit.hpp"

namespace x86_64 {

template<typename MemoryManagementUnit>
class PhysicalMemoryMap : public i386::GenericPhysicalMemoryMap<x86_64::PagingResolver<MemoryManagementUnit>> {
public:
    PhysicalMemoryMap(MemoryManagementUnit& mmu)
     : i386::GenericPhysicalMemoryMap<x86_64::PagingResolver<MemoryManagementUnit>>(mmu)
    {}
    
    void populate_page_tables([[maybe_unused]] uintptr_t begin,
                              [[maybe_unused]] size_t length) override
    {
        assert(!"stub");
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP