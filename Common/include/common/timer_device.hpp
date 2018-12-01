#ifndef FLAPJACKOS_KERNEL_INCLUDE_TIMER_DEVICE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_TIMER_DEVICE_HPP

class TimerDevice {
public:
    virtual ~TimerDevice() = default;

    // Gets the number of timer ticks since boot.
    virtual unsigned ticks() = 0;

    // Gets the number of seconds since boot.
    virtual unsigned seconds() = 0;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_TIMER_DEVICE_HPP
