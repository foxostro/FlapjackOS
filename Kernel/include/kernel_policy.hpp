#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP

#if defined(__x86_64__)

#include <platform/x86_64/hardware_task_configuration.hpp>
#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <platform/x86_64/hardware_memory_management_unit.hpp>
#include <platform/x86_64/physical_memory_map.hpp>
#include <platform/x86_64/interrupt_controller.hpp>
#include <platform/x86_64/device_drivers.hpp>
#include <drivers/pc/vga_text_display_device.hpp>
#include <platform/x86_64/thread.hpp>

struct KernelPolicy {
    using HardwareTaskConfiguration = x86_64::HardwareTaskConfiguration;
    using HardwareMemoryManagementUnit = x86_64::HardwareMemoryManagementUnit;
    using KernelAddressSpaceBootstrapper = x86_64::KernelAddressSpaceBootstrapper<x86_64::HardwareMemoryManagementUnit>;
    using PhysicalMemoryMap = x86_64::PhysicalMemoryMap<x86_64::HardwareMemoryManagementUnit>;
    using InterruptController = x86_64::InterruptController;
    using InterruptParameters = x86_64::InterruptParameters;
    using DeviceDrivers = x86_64::DeviceDrivers;
    using TextDisplayDevice = VGATextDisplayDevice;
    using Thread = x86_64::Thread_x86_64;
    using ThreadExternalStack = x86_64::Thread_x86_64_ExternalStack;
};

#elif defined(__i386__)

#include <platform/i386/hardware_task_configuration.hpp>
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <platform/i386/hardware_memory_management_unit.hpp>
#include <platform/i386/physical_memory_map.hpp>
#include <platform/i386/interrupt_controller.hpp>
#include <platform/i386/device_drivers.hpp>
#include <drivers/pc/vga_text_display_device.hpp>
#include <platform/i386/thread.hpp>

struct KernelPolicy {
    using HardwareTaskConfiguration = i386::HardwareTaskConfiguration;
    using HardwareMemoryManagementUnit = i386::HardwareMemoryManagementUnit;
    using KernelAddressSpaceBootstrapper = i386::KernelAddressSpaceBootstrapper<i386::HardwareMemoryManagementUnit>;
    using PhysicalMemoryMap = i386::PhysicalMemoryMap<i386::HardwareMemoryManagementUnit>;
    using InterruptController = i386::InterruptController;
    using InterruptParameters = i386::InterruptParameters;
    using DeviceDrivers = i386::DeviceDrivers;
    using TextDisplayDevice = VGATextDisplayDevice;
    using Thread = i386::Thread_i386;
    using ThreadExternalStack = i386::Thread_i386_ExternalStack;
};

#elif defined(__arm__)

#error "The ARM-specific MMU classes have not been written yet."

#else

#error "unknown platform"

#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP