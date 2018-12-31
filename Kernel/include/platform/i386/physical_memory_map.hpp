#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP

#include "generic_physical_memory_map.hpp"
#include "paging_resolver.hpp"
#include "hardware_memory_management_unit.hpp"

namespace i386 {

template<typename MemoryManagementUnit>
class PhysicalMemoryMap : public GenericPhysicalMemoryMap<i386::PagingResolver<MemoryManagementUnit>>
{
public:
    PhysicalMemoryMap(MemoryManagementUnit& mmu)
     : GenericPhysicalMemoryMap<i386::PagingResolver<MemoryManagementUnit>>(mmu)
    {}
    
    void populate_page_tables([[maybe_unused]] uintptr_t begin,
                              [[maybe_unused]] size_t length) override
    {
        assert(!"stub");
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP