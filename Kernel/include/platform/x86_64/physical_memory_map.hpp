#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/generic_physical_memory_map.hpp>
#include "paging_resolver.hpp"

namespace x86_64 {

template<typename MMU>
using PhysicalMemoryMap = i386::GenericPhysicalMemoryMap< x86_64::PagingResolver<MMU> >;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PHYSICAL_MEMORY_MAP_HPP