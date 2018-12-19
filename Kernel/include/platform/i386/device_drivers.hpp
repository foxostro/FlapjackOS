#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_DEVICE_DRIVERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_DEVICE_DRIVERS_HPP

#include <platform/i386/interrupt_controller.hpp>
#include <drivers/pc/ps2_keyboard_device.hpp>
#include <drivers/pc/pit_timer_device.hpp>

namespace i386 {

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

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_DEVICE_DRIVERS_HPP
