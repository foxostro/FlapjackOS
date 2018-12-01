#include <pit_timer_device.hpp>

#include <pic.h>
#include <inout.h>
#include <common/misc.h>

constexpr unsigned TIMER_PERIOD_IO_PORT = 0x40;
constexpr unsigned TIMER_MODE_IO_PORT   = 0x43;
constexpr unsigned TIMER_SQUARE_WAVE    = 0x36;

void PITTimerDevice::int_handler(const ParameterPackage&)
{
    if (++leap_counter_ > timer_leap_interval_) {
        leap_counter_ = 0; // reset
        clock_ticks_ += timer_leap_ticks_;
    }

    clock_ticks_ += timer_rate_;
    if (clock_ticks_ > CLOCK_FREQUENCY) {
        clock_seconds_ += clock_ticks_ / CLOCK_FREQUENCY;
        clock_ticks_ = clock_ticks_ % CLOCK_FREQUENCY;
    }
}

unsigned PITTimerDevice::ticks()
{
    return clock_ticks_;
}

unsigned PITTimerDevice::seconds()
{
    return clock_seconds_;
}

PITTimerDevice::PITTimerDevice(unsigned short _timer_rate,
                               int leap_interval,
                               int leap_ticks)
: timer_rate_(_timer_rate),
  timer_leap_interval_(leap_interval),
  timer_leap_ticks_(leap_ticks)
{
    outb(TIMER_MODE_IO_PORT, TIMER_SQUARE_WAVE);
    outb(TIMER_PERIOD_IO_PORT, WORD_LOWER_BYTE(timer_rate_));
    outb(TIMER_PERIOD_IO_PORT, WORD_UPPER_BYTE(timer_rate_));
}
