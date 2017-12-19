#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <interfaces/keyboard_interface.h>

#define BUFFER_SIZE (32)

typedef struct ps2_keyboard {
    void (*destroy)(struct ps2_keyboard *self);
    void (*int_handler)(struct ps2_keyboard *self);
    const char* (*keycode_name)(keycode_t key);
    void (*get_event)(struct ps2_keyboard *self, keyboard_event_t *output);

    keycode_key_state_t key_state[KEYCODE_MAX];
    keyboard_event_t ring_buffer[BUFFER_SIZE];
    size_t dequeue_pos;
    size_t enqueue_pos;
    size_t count;
} ps2_keyboard_t;

// Initializes the keyboard device and driver.
keyboard_interface_t* ps2_keyboard_init(void);

#ifdef __cplusplus
}
#endif