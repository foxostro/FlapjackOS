#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/generic_unmanaged_physical_memory_map.hpp>
#include "paging_resolver.hpp"
#include <hardware_memory_management_unit.hpp>

namespace x86_64 {

class UnmanagedPhysicalMemoryMap : public i386::GenericUnmanagedPhysicalMemoryMap<PagingResolver> {
public:
    UnmanagedPhysicalMemoryMap(HardwareMemoryManagementUnit& mmu)
     : i386::GenericUnmanagedPhysicalMemoryMap<PagingResolver>(mmu)
    {}
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_UNMANAGED_PHYSICAL_MEMORY_MAP_HPP