#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CONCRETE_PHYSICAL_MEMORY_MAP_HPP

#include <platform/i386/generic_concrete_physical_memory_map.hpp>
#include <platform/x86_64/unmanaged_physical_memory_map.hpp>
#include <platform/x86_64/managed_physical_memory_map.hpp>

namespace x86_64 {

struct ConcretePhysicalMemoryMapPolicy {
    using UnmanagedPhysicalMemoryMap = x86_64::UnmanagedPhysicalMemoryMap;
    using ManagedPhysicalMemoryMap = x86_64::ManagedPhysicalMemoryMap;
};

using ConcretePhysicalMemoryMap = i386::GenericConcretePhysicalMemoryMap<ConcretePhysicalMemoryMapPolicy>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_CONCRETE_PHYSICAL_MEMORY_MAP_HPP
