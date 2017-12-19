#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <interfaces/keyboard_interface.h>

#define BUFFER_SIZE (32)

typedef struct ps2_keyboard {
    void (*destroy)(struct ps2_keyboard *this);
    void (*int_handler)(struct ps2_keyboard *this);
    const char* (*keycode_name)(keycode_t key);
    void (*get_event)(struct ps2_keyboard *this, keyboard_event_t *output);

    keycode_key_state_t key_state[KEYCODE_MAX];
    keyboard_event_t ring_buffer[BUFFER_SIZE];
    size_t dequeue_pos;
    size_t enqueue_pos;
    size_t count;
} ps2_keyboard_t;

_Static_assert(offsetof(ps2_keyboard_t, destroy) == offsetof(keyboard_interface_t, destroy),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(ps2_keyboard_t, int_handler) == offsetof(keyboard_interface_t, int_handler),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(ps2_keyboard_t, keycode_name) == offsetof(keyboard_interface_t, keycode_name),
    "Incorrect offset prevents correct casting behavior when casting between types.");

_Static_assert(offsetof(ps2_keyboard_t, get_event) == offsetof(keyboard_interface_t, get_event),
    "Incorrect offset prevents correct casting behavior when casting between types.");

// Initializes the keyboard device and driver.
keyboard_interface_t* ps2_keyboard_init(void);

#ifdef __cplusplus
}
#endif