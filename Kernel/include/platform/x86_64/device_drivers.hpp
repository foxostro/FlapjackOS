#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_DEVICE_DRIVERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_DEVICE_DRIVERS_HPP

#include <platform/x86_64/interrupt_controller.hpp>
#include <drivers/pc/ps2_keyboard_device.hpp>
#include <drivers/pc/pit_timer_device.hpp>

namespace x86_64 {

// TODO: This class is very similar to i386::DeviceDrivers. Can the two be consolidated?
class DeviceDrivers {
public:
    DeviceDrivers(InterruptController& interrupt_controller);
    ~DeviceDrivers();
    void init();

    inline KeyboardDevice& get_keyboard()
    {
        assert(keyboard_device_);
        return *keyboard_device_;
    }

private:
    InterruptController& interrupt_controller_;
    PS2KeyboardDevice* keyboard_device_;
    PITTimerDevice* timer_device_;
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_DEVICE_DRIVERS_HPP
