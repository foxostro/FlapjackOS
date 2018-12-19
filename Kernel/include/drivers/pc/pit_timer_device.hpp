#ifndef FLAPJACKOS_KERNEL_INCLUDE_PIT_TIMER_DEVICE_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PIT_TIMER_DEVICE_HPP

#include <common/timer_device.hpp>

class PITTimerDevice : public TimerDevice {
public:
    static constexpr unsigned CLOCK_FREQUENCY          = 1193182; // Clock cycles per second.
    static constexpr unsigned TIMER_RATE_10ms          = 11931;
    static constexpr unsigned TIMER_LEAP_INTERVAL_10ms = 50;
    static constexpr unsigned TIMER_LEAP_TICKS_10ms    = 41;

    // Configure the PIT timer. Set the rate of timer interrupts.
    // timer_rate -- clock cycles between timer interrupts
    // leap_interval -- Every few timer interrupts, it may be necessary to add
    // a few leap ticks to the tick count to keep the timer accurate. This is the
    // interval between such leap ticks.
    // leap_ticks -- Every few timer interrupts, it may be necessary to add
    // a few leap ticks to the tick count to keep the timer accurate. This is the
    // number of leap ticks that will be added at such a time.
    PITTimerDevice(unsigned short timer_rate,
                   int leap_interval,
                   int leap_ticks);

    // Timer interrupt handler. To be called when the timer interrupt fires.
    void on_interrupt() noexcept;

    unsigned ticks() override;
    unsigned seconds() override;
    
private:
    unsigned clock_ticks_;
    unsigned clock_seconds_;
    unsigned leap_counter_;
    unsigned timer_rate_;
    unsigned timer_leap_interval_;
    unsigned timer_leap_ticks_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PIT_TIMER_DEVICE_HPP
