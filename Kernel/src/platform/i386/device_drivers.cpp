#include <platform/i386/device_drivers.hpp>
#include <platform/i386/interrupt_parameters.hpp>
#include <simple_device_interrupt_handler.hpp>

namespace i386 {

DeviceDrivers::DeviceDrivers(InterruptController& interrupt_controller)
 : interrupt_controller_(interrupt_controller),
   keyboard_device_(nullptr),
   timer_device_(nullptr)
{

}

DeviceDrivers::~DeviceDrivers()
{
    delete keyboard_device_; // AFOX_TODO: Need something like std::unique_ptr here. Or perhaps std::optional if these do not need to be heap allocated.
    delete timer_device_;
}

void DeviceDrivers::init()
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

} // namespace i386
