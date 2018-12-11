#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/generic_physical_memory_map.hpp>
#include "paging_resolver.hpp"

namespace x86_64 {

using PhysicalMemoryMap = i386::GenericPhysicalMemoryMap<x86_64::PagingResolver>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP