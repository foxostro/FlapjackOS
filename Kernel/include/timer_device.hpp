#pragma once

class timer_device {
public:
    virtual ~timer_device() = default;

    // Gets the number of timer ticks since boot.
    virtual unsigned ticks() = 0;

    // Gets the number of seconds since boot.
    virtual unsigned seconds() = 0;
};
