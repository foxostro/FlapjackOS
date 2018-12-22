#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_DEVICE_DRIVERS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_DEVICE_DRIVERS_HPP

#include <platform/i386/interrupt_controller.hpp>
#include <platform/i386/interrupt_parameters.hpp>
#include <drivers/pc/ps2_keyboard_device.hpp>
#include <drivers/pc/pit_timer_device.hpp>
#include <simple_device_interrupt_handler.hpp>

namespace i386 {

template<typename Policy>
class GenericDeviceDrivers {
public:
    using InterruptParameters = typename Policy::InterruptParameters;
    using InterruptController = typename Policy::InterruptController;

    GenericDeviceDrivers(InterruptController& interrupt_controller)
     : interrupt_controller_(interrupt_controller),
       keyboard_device_(nullptr),
       timer_device_(nullptr)
    {}

    ~GenericDeviceDrivers()
    {
        delete keyboard_device_; // AFOX_TODO: Need something like std::unique_ptr here. Or perhaps std::optional if these do not need to be heap allocated.
        delete timer_device_;
    }

    void init()
    {
        // Keyboard
        keyboard_device_ = new PS2KeyboardDevice();
        auto keyboard_handler = new SimpleDeviceInterruptHandler<InterruptParameters, PS2KeyboardDevice>(*keyboard_device_);
        interrupt_controller_.install(IDT_KEY, keyboard_handler);

        // Timer
        timer_device_ = new PITTimerDevice(PITTimerDevice::TIMER_RATE_10ms,
                                        PITTimerDevice::TIMER_LEAP_INTERVAL_10ms,
                                        PITTimerDevice::TIMER_LEAP_TICKS_10ms);
        auto timer_handler = new SimpleDeviceInterruptHandler<InterruptParameters, PITTimerDevice>(*timer_device_);
        interrupt_controller_.install(IDT_TIMER, timer_handler);
    }

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

struct DeviceDriversPolicy {
    using InterruptParameters = i386::InterruptParameters;
    using InterruptController = i386::InterruptController;
};

using DeviceDrivers = GenericDeviceDrivers<DeviceDriversPolicy>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_DEVICE_DRIVERS_HPP
