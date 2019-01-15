#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP

#include "generic_unmanaged_physical_memory_map.hpp"
#include "paging_resolver.hpp"
#include <hardware_memory_management_unit.hpp>
#include <common/logger.hpp>

namespace i386 {

class UnmanagedPhysicalMemoryMap : public GenericUnmanagedPhysicalMemoryMap<PagingResolver>
{
public:
    UnmanagedPhysicalMemoryMap(HardwareMemoryManagementUnit& mmu)
     : GenericUnmanagedPhysicalMemoryMap<PagingResolver>(mmu)
    {}
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP