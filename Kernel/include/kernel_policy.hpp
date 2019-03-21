#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP

#if defined(__x86_64__)

#include <platform/x86_64/hardware_task_configuration.hpp>
#include <platform/x86_64/kernel_address_space_bootstrapper.hpp>
#include <platform/x86_64/concrete_hardware_memory_management_unit.hpp>
#include <platform/x86_64/concrete_physical_memory_map.hpp>
#include <platform/x86_64/interrupt_controller.hpp>
#include <platform/x86_64/device_drivers.hpp>
#include <drivers/pc/vga_text_display_device.hpp>
#include <platform/x86_64/thread.hpp>
#include <platform/x86_64/platform_namer.hpp>
#include <drivers/pc/serial.hpp>
#include <platform/x86_64/floating_point_features_initializer.hpp>

struct KernelPolicy {
    using HardwareTaskConfiguration = x86_64::HardwareTaskConfiguration;
    using ConcreteHardwareMemoryManagementUnit = x86_64::ConcreteHardwareMemoryManagementUnit;
    using KernelAddressSpaceBootstrapper = x86_64::KernelAddressSpaceBootstrapper;
    using ConcretePhysicalMemoryMap = x86_64::ConcretePhysicalMemoryMap;
    using HardwareInterruptController = x86_64::HardwareInterruptController;
    using InterruptController = x86_64::InterruptController;
    using InterruptParameters = x86_64::InterruptParameters;
    using DeviceDrivers = x86_64::DeviceDrivers;
    using TextDisplayDevice = VGATextDisplayDevice;
    using Thread = x86_64::Thread_x86_64;
    using ThreadExternalStack = x86_64::Thread_x86_64_ExternalStack;
    using PlatformNamer = x86_64::PlatformNamer;
    using LoggerTextOutputStream = Serial;
    using FloatingPointFeaturesInitializer = x86_64::FloatingPointFeaturesInitializer;
};

#elif defined(__i386__)

#include <platform/i386/hardware_task_configuration.hpp>
#include <platform/i386/kernel_address_space_bootstrapper.hpp>
#include <platform/i386/concrete_hardware_memory_management_unit.hpp>
#include <platform/i386/concrete_physical_memory_map.hpp>
#include <platform/i386/interrupt_controller.hpp>
#include <platform/i386/device_drivers.hpp>
#include <drivers/pc/vga_text_display_device.hpp>
#include <platform/i386/thread.hpp>
#include <platform/i386/platform_namer.hpp>
#include <drivers/pc/serial.hpp>
#include <platform/i386/floating_point_features_initializer.hpp>

struct KernelPolicy {
    using HardwareTaskConfiguration = i386::HardwareTaskConfiguration;
    using ConcreteHardwareMemoryManagementUnit = i386::ConcreteHardwareMemoryManagementUnit;
    using KernelAddressSpaceBootstrapper = i386::KernelAddressSpaceBootstrapper;
    using ConcretePhysicalMemoryMap = i386::ConcretePhysicalMemoryMap;
    using HardwareInterruptController = i386::HardwareInterruptController;
    using InterruptController = i386::InterruptController;
    using InterruptParameters = i386::InterruptParameters;
    using DeviceDrivers = i386::DeviceDrivers;
    using TextDisplayDevice = VGATextDisplayDevice;
    using Thread = i386::Thread_i386;
    using ThreadExternalStack = i386::Thread_i386_ExternalStack;
    using PlatformNamer = i386::PlatformNamer;
    using LoggerTextOutputStream = Serial;
    using FloatingPointFeaturesInitializer = i386::FloatingPointFeaturesInitializer;
};

#elif defined(__arm__)

#error "The ARM-specific MMU classes have not been written yet."

#else

#error "unknown platform"

#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_POLICY_HPP