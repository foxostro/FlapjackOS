#include <pit_timer_device.hpp>
#include <stdint.h>
#include <pic.h>
#include <inout.h>
#include <misc.h>
#include <stdlib.h>
#include <assert.h>

#define TIMER_PERIOD_IO_PORT (0x40)
#define TIMER_MODE_IO_PORT   (0x43)
#define TIMER_SQUARE_WAVE    (0x36)

void pit_timer_device::int_handler()
{
    if (++leap_counter > timer_leap_interval) {
        leap_counter = 0; // reset
        clock_ticks += timer_leap_ticks;
    }

    clock_ticks += timer_rate;
    if (clock_ticks > CLOCK_FREQUENCY) {
        clock_seconds += clock_ticks / CLOCK_FREQUENCY;
        clock_ticks = clock_ticks % CLOCK_FREQUENCY;
    }
}

unsigned pit_timer_device::ticks()
{
    return clock_ticks;
}

unsigned pit_timer_device::seconds()
{
    return clock_seconds;
}

pit_timer_device::pit_timer_device(unsigned short _timer_rate,
                                   int leap_interval,
                                   int leap_ticks)
: timer_rate(_timer_rate),
  timer_leap_interval(leap_interval),
  timer_leap_ticks(leap_ticks)
{
    outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
    outb(TIMER_PERIOD_IO_PORT, WORD_LOWER_BYTE(timer_rate));
    outb(TIMER_PERIOD_IO_PORT, WORD_UPPER_BYTE(timer_rate));
}
