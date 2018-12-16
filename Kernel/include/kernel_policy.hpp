#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP

#if defined(__x86_64__)

#include <platform/x86_64/hardware_task_configuration.hpp>
#include <platform/x86_64/hardware_interrupt_controller.hpp>
#include <generic_interrupt_dispatcher.hpp>
#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <platform/x86_64/hardware_memory_management_unit.hpp>
#include <platform/x86_64/physical_memory_map.hpp>
#include <drivers/pc/vga_text_display_device.hpp>

struct KernelPolicy {
	using HardwareTaskConfiguration = x86_64::HardwareTaskConfiguration;
	using HardwareInterruptController = x86_64::HardwareInterruptController;
	using InterruptDispatcher = GenericInterruptDispatcher<x86_64::HardwareInterruptController>;
	using HardwareMemoryManagementUnit = x86_64::HardwareMemoryManagementUnit;
	using KernelAddressSpaceBootstrapper = x86_64::KernelAddressSpaceBootstrapper<x86_64::HardwareMemoryManagementUnit>;
	using PhysicalMemoryMap = x86_64::PhysicalMemoryMap<x86_64::HardwareMemoryManagementUnit>;
	using TextDisplayDevice = VGATextDisplayDevice;
};

#elif defined(__i386__)

#include <platform/i386/hardware_task_configuration.hpp>
#include <platform/i386/hardware_interrupt_controller.hpp>
#include <generic_interrupt_dispatcher.hpp>
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <platform/i386/hardware_memory_management_unit.hpp>
#include <platform/i386/physical_memory_map.hpp>
#include <drivers/pc/vga_text_display_device.hpp>

struct KernelPolicy {
	using HardwareTaskConfiguration = i386::HardwareTaskConfiguration;
	using HardwareInterruptController = i386::HardwareInterruptController;
	using InterruptDispatcher = GenericInterruptDispatcher<i386::HardwareInterruptController>;
	using HardwareMemoryManagementUnit = i386::HardwareMemoryManagementUnit;
	using KernelAddressSpaceBootstrapper = i386::KernelAddressSpaceBootstrapper<i386::HardwareMemoryManagementUnit>;
	using PhysicalMemoryMap = i386::PhysicalMemoryMap<i386::HardwareMemoryManagementUnit>;
	using TextDisplayDevice = VGATextDisplayDevice;
};

#elif defined(__arm__)

#error "The ARM-specific MMU classes have not been written yet."

#else

#error "unknown platform"

#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP