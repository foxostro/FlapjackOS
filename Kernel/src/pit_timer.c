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

static void pit_timer_int_handler(pit_timer_t *self)
{
    assert(self);

    if (++self->leap_counter > self->timer_leap_interval) {
        self->leap_counter = 0; // reset
        self->clock_ticks += self->timer_leap_ticks;
    }

    self->clock_ticks += self->timer_rate;
    if (self->clock_ticks > CLOCK_FREQUENCY) {
        self->clock_seconds += self->clock_ticks / CLOCK_FREQUENCY;
        self->clock_ticks = self->clock_ticks % CLOCK_FREQUENCY;
    }
}

static unsigned pit_timer_ticks(pit_timer_t *self)
{
    assert(self);
    return self->clock_ticks;
}

static unsigned pit_timer_seconds(pit_timer_t *self)
{
    assert(self);
    return self->clock_seconds;
}

timer_interface_t* pit_timer_init(unsigned short timer_rate,
                                  int leap_interval,
                                  int leap_ticks)
{
    pit_timer_t *self = malloc(sizeof(pit_timer_t));

    self->int_handler = pit_timer_int_handler;
    self->ticks = pit_timer_ticks;
    self->seconds = pit_timer_seconds;

    self->timer_rate = timer_rate;
    self->timer_leap_interval = leap_interval;
    self->timer_leap_ticks = leap_ticks;

    outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
    outb(TIMER_PERIOD_IO_PORT, WORD_LOWER_BYTE(timer_rate));
    outb(TIMER_PERIOD_IO_PORT, WORD_UPPER_BYTE(timer_rate));

    return (timer_interface_t *)self;
}
