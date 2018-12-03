#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP

#if defined(__x86_64__)

// TODO: Write platform-specific MMU classes for x86_64.
#include <platform/x86_64/hardware_task_configuration.hpp>
#include <platform/i386/hardware_interrupt_controller.hpp>
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <platform/i386/physical_memory_map.hpp>
#include <drivers/pc/vga_text_display_device.hpp>

struct KernelPolicy {
	using HardwareTaskConfiguration = x86_64::HardwareTaskConfiguration;
	using HardwareInterruptController = i386::HardwareInterruptController;
	using KernelAddressSpaceBootstrapper = i386::KernelAddressSpaceBootstrapper;
	using PhysicalMemoryMap = i386::PhysicalMemoryMap;
	using TextDisplayDevice = VGATextDisplayDevice;
};

#elif defined(__i386__)

#include <platform/i386/hardware_task_configuration.hpp>
#include <platform/i386/hardware_interrupt_controller.hpp>
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <platform/i386/physical_memory_map.hpp>
#include <drivers/pc/vga_text_display_device.hpp>

struct KernelPolicy {
	using HardwareTaskConfiguration = i386::HardwareTaskConfiguration;
	using HardwareInterruptController = i386::HardwareInterruptController;
	using KernelAddressSpaceBootstrapper = i386::KernelAddressSpaceBootstrapper;
	using PhysicalMemoryMap = i386::PhysicalMemoryMap;
	using TextDisplayDevice = VGATextDisplayDevice;
};

#elif defined(__arm__)

#error "The ARM-specific MMU classes have not been written yet."

#else

#error "unknown platform"

#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP