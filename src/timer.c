#include <timer.h>
#include <stdint.h>
#include <pic.h>
#include <inout.h>
#include <misc.h>
#include <kprintf.h>

#define TIMER_PERIOD_IO_PORT 0x40
#define TIMER_MODE_IO_PORT   0x43
#define TIMER_SQUARE_WAVE    0x36

static unsigned s_clock_ticks;
static unsigned s_clock_seconds;

static unsigned s_leap_counter;
static unsigned s_timer_rate;
static unsigned s_timer_leap_interval;
static unsigned s_timer_leap_ticks;

void timer_int_handler()
{
	if(++s_leap_counter > s_timer_leap_interval) {
		s_leap_counter = 0; // reset
		s_clock_ticks += s_timer_leap_ticks;
		kprintf("leap tick! -- timer_ticks = %u\n", s_clock_ticks);
	}

	s_clock_ticks += s_timer_rate;
	if(s_clock_ticks > CLOCK_FREQUENCY) {
		s_clock_seconds += s_clock_ticks / CLOCK_FREQUENCY;
		s_clock_ticks = s_clock_ticks % CLOCK_FREQUENCY;
	}

	pic_clear();
}

void timer_init(unsigned short timer_rate, int leap_interval, int leap_ticks)
{
	s_timer_rate = timer_rate;
	s_timer_leap_interval = leap_interval;
	s_timer_leap_ticks = leap_ticks;

	outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
	outb(TIMER_PERIOD_IO_PORT, WORD_LOWER_BYTE(timer_rate));
	outb(TIMER_PERIOD_IO_PORT, WORD_UPPER_BYTE(timer_rate));
}

unsigned timer_ticks()
{
	return s_clock_ticks;
}

unsigned timer_seconds()
{
	return s_clock_seconds;
}
