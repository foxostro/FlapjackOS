#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_DEVICE_DRIVERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_DEVICE_DRIVERS_HPP

#include <platform/i386/device_drivers.hpp>
#include <platform/x86_64/interrupt_controller.hpp>

namespace x86_64 {

struct DeviceDriversPolicy {
    using InterruptParameters = x86_64::InterruptParameters;
    using InterruptController = x86_64::InterruptController;
};

using DeviceDrivers = i386::GenericDeviceDrivers<DeviceDriversPolicy>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_DEVICE_DRIVERS_HPP
