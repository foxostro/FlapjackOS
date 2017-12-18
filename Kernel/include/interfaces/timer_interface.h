#pragma once

typedef struct timer {
    // Timer interrupt handler. Called by the interrupt dispatcher.
    void (*int_handler)(struct timer *this);

    // Gets the number of timer ticks since boot.
    unsigned (*ticks)(struct timer *this);

    // Gets the number of seconds since boot.
    unsigned (*seconds)(struct timer *this);

    // Frees memory associated with the timer object.
    void (*destroy)(struct timer *this);
} timer_interface_t;