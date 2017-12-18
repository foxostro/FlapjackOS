#include <pit_timer.h>
#include <stdint.h>
#include <pic.h>
#include <inout.h>
#include <misc.h>
#include <stdlib.h>
#include <assert.h>

#define TIMER_PERIOD_IO_PORT (0x40)
#define TIMER_MODE_IO_PORT   (0x43)
#define TIMER_SQUARE_WAVE    (0x36)

static void pit_timer_int_handler(pit_timer_t *this)
{
    assert(this);

    if (++this->leap_counter > this->timer_leap_interval) {
        this->leap_counter = 0; // reset
        this->clock_ticks += this->timer_leap_ticks;
    }

    this->clock_ticks += this->timer_rate;
    if (this->clock_ticks > CLOCK_FREQUENCY) {
        this->clock_seconds += this->clock_ticks / CLOCK_FREQUENCY;
        this->clock_ticks = this->clock_ticks % CLOCK_FREQUENCY;
    }
}

static unsigned pit_timer_ticks(pit_timer_t *this)
{
    assert(this);
    return this->clock_ticks;
}

static unsigned pit_timer_seconds(pit_timer_t *this)
{
    assert(this);
    return this->clock_seconds;
}

timer_interface_t* pit_timer_init(unsigned short timer_rate,
                                  int leap_interval,
                                  int leap_ticks)
{
    pit_timer_t *this = malloc(sizeof(pit_timer_t));

    this->int_handler = pit_timer_int_handler;
    this->ticks = pit_timer_ticks;
    this->seconds = pit_timer_seconds;

    this->timer_rate = timer_rate;
    this->timer_leap_interval = leap_interval;
    this->timer_leap_ticks = leap_ticks;

    outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
    outb(TIMER_PERIOD_IO_PORT, WORD_LOWER_BYTE(timer_rate));
    outb(TIMER_PERIOD_IO_PORT, WORD_UPPER_BYTE(timer_rate));

    return (timer_interface_t *)this;
}
