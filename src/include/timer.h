#pragma once

#define CLOCK_FREQUENCY          1193182 // Clock cycles per second.
#define TIMER_RATE_10ms          11931
#define TIMER_LEAP_INTERVAL_10ms 50
#define TIMER_LEAP_TICKS_10ms    41
#define TIMER_IDT_ENTRY          0x20

// Set the rate of timer interrupts.
// timer_rate -- clock cycles between timer interrupts
// leap_interval -- Every few timer interrupts, it may be necessary to add
// a few leap ticks to the tick count to keep the timer accurate. This is the
// interval between such leap ticks.
// leap_ticks -- Every few timer interrupts, it may be necessary to add
// a few leap ticks to the tick count to keep the timer accurate. This is the
// number of leap ticks that will be added at such a time.
void timer_init(unsigned short timer_rate, int leap_interval, int leap_ticks);

// Gets the number of timer ticks since boot.
unsigned timer_ticks(void);

// Gets the number of seconds since boot.
unsigned timer_seconds(void);
