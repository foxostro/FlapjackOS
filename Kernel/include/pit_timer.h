#pragma once

#include <stddef.h>
#include <interfaces/timer_interface.h>

#define CLOCK_FREQUENCY          (1193182) // Clock cycles per second.
#define TIMER_RATE_10ms          (11931)
#define TIMER_LEAP_INTERVAL_10ms (50)
#define TIMER_LEAP_TICKS_10ms    (41)

typedef struct pit_timer {
    void (*int_handler)(struct pit_timer *this);
    unsigned (*ticks)(struct pit_timer *this);
    unsigned (*seconds)(struct pit_timer *this);
    void (*destroy)(struct pit_timer *this);

    unsigned clock_ticks;
    unsigned clock_seconds;
    unsigned leap_counter;
    unsigned timer_rate;
    unsigned timer_leap_interval;
    unsigned timer_leap_ticks;
} pit_timer_t;

_Static_assert(offsetof(pit_timer_t, int_handler) == offsetof(timer_interface_t, int_handler),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(pit_timer_t, ticks) == offsetof(timer_interface_t, ticks),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(pit_timer_t, seconds) == offsetof(timer_interface_t, seconds),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(pit_timer_t, destroy) == offsetof(timer_interface_t, destroy),
    "Incorrect offset prevents correct casting behavior when casting between types.");

// Configure the PIT timer. Set the rate of timer interrupts.
// timer_rate -- clock cycles between timer interrupts
// leap_interval -- Every few timer interrupts, it may be necessary to add
// a few leap ticks to the tick count to keep the timer accurate. This is the
// interval between such leap ticks.
// leap_ticks -- Every few timer interrupts, it may be necessary to add
// a few leap ticks to the tick count to keep the timer accurate. This is the
// number of leap ticks that will be added at such a time.
timer_interface_t* pit_timer_init(unsigned short timer_rate,
                                  int leap_interval,
                                  int leap_ticks);
