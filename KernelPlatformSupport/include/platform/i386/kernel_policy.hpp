#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_POLICY_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_POLICY_HPP

#include "hardware_task_configuration.hpp"
#include "kernel_address_space_bootstrapper.hpp"
#include "physical_memory_map.hpp"

namespace i386 {

struct KernelPolicy {
	using HardwareTaskConfiguration = i386::HardwareTaskConfiguration;
	using KernelAddressSpaceBootstrapper = i386::KernelAddressSpaceBootstrapper;
	using PhysicalMemoryMap = i386::PhysicalMemoryMap;
};

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_KERNEL_POLICY_HPP