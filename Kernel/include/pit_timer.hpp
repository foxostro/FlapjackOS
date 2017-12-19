#pragma once

#include <stddef.h>
#include <timer.hpp>

#define CLOCK_FREQUENCY          (1193182) // Clock cycles per second.
#define TIMER_RATE_10ms          (11931)
#define TIMER_LEAP_INTERVAL_10ms (50)
#define TIMER_LEAP_TICKS_10ms    (41)

class pit_timer : public timer {
public:
    // Configure the PIT timer. Set the rate of timer interrupts.
    // timer_rate -- clock cycles between timer interrupts
    // leap_interval -- Every few timer interrupts, it may be necessary to add
    // a few leap ticks to the tick count to keep the timer accurate. This is the
    // interval between such leap ticks.
    // leap_ticks -- Every few timer interrupts, it may be necessary to add
    // a few leap ticks to the tick count to keep the timer accurate. This is the
    // number of leap ticks that will be added at such a time.
    pit_timer(unsigned short timer_rate,
              int leap_interval,
              int leap_ticks);

    void int_handler() override;
    unsigned ticks() override;
    unsigned seconds() override;

private:
    unsigned clock_ticks;
    unsigned clock_seconds;
    unsigned leap_counter;
    unsigned timer_rate;
    unsigned timer_leap_interval;
    unsigned timer_leap_ticks;
};