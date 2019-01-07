#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/generic_concrete_physical_memory_map.hpp>
#include <platform/i386/unmanaged_physical_memory_map.hpp>
#include <platform/i386/managed_physical_memory_map.hpp>

namespace i386 {

struct ConcretePhysicalMemoryMapPolicy {
    using UnmanagedPhysicalMemoryMap = i386::UnmanagedPhysicalMemoryMap;
    using ManagedPhysicalMemoryMap = i386::ManagedPhysicalMemoryMap;
};

using ConcretePhysicalMemoryMap = i386::GenericConcretePhysicalMemoryMap<ConcretePhysicalMemoryMapPolicy>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
