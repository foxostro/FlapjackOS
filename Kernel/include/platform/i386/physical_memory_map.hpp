#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP

#include "generic_physical_memory_map.hpp"
#include "paging_resolver.hpp"

namespace i386 {

template<typename MMU>
using PhysicalMemoryMap = GenericPhysicalMemoryMap< i386::PagingResolver<MMU> >;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PHYSICAL_MEMORY_MAP_HPP