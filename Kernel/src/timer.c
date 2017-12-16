#include <timer.h>
#include <stdint.h>
#include <pic.h>
#include <inout.h>
#include <misc.h>

#define TIMER_PERIOD_IO_PORT 0x40
#define TIMER_MODE_IO_PORT   0x43
#define TIMER_SQUARE_WAVE    0x36

static unsigned s_clock_ticks;
static unsigned s_clock_seconds;

static unsigned s_leap_counter;
static unsigned s_timer_rate;
static unsigned s_timer_leap_interval;
static unsigned s_timer_leap_ticks;

static void timer_int_handler(void)
{
    if (++s_leap_counter > s_timer_leap_interval) {
        s_leap_counter = 0; // reset
        s_clock_ticks += s_timer_leap_ticks;
    }

    s_clock_ticks += s_timer_rate;
    if (s_clock_ticks > CLOCK_FREQUENCY) {
        s_clock_seconds += s_clock_ticks / CLOCK_FREQUENCY;
        s_clock_ticks = s_clock_ticks % CLOCK_FREQUENCY;
    }
}

static void timer_init(unsigned short timer_rate, int leap_interval, int leap_ticks)
{
    s_timer_rate = timer_rate;
    s_timer_leap_interval = leap_interval;
    s_timer_leap_ticks = leap_ticks;

    outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
    outb(TIMER_PERIOD_IO_PORT, WORD_LOWER_BYTE(timer_rate));
    outb(TIMER_PERIOD_IO_PORT, WORD_UPPER_BYTE(timer_rate));
}

static unsigned timer_ticks(void)
{
    return s_clock_ticks;
}

static unsigned timer_seconds(void)
{
    return s_clock_seconds;
}

void get_timer_interface(timer_interface_t *timer)
{
    timer->int_handler = timer_int_handler;
    timer->init = timer_init;
    timer->ticks = timer_ticks;
    timer->seconds = timer_seconds;
}
