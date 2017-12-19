#pragma once

#include <interfaces/keyboard_device.hpp>
#include <stddef.h>

class ps2_keyboard_device : public keyboard_device {
    static constexpr size_t BUFFER_SIZE = 32;

    typedef enum {
        IDLE,
        PROCESSING_ESCAPE_CODE
    } keyboard_driver_state_t;

    keycode_key_state_t key_state[KEYCODE_MAX];
    keyboard_event_t ring_buffer[BUFFER_SIZE];
    size_t dequeue_pos;
    size_t enqueue_pos;
    size_t count;

    void enqueue(keyboard_event_t *event);
    bool dequeue(keyboard_event_t *event);
    void clear_input();
    bool step_state_machine(keyboard_driver_state_t *state, keyboard_event_t *output);

public:
    ps2_keyboard_device();

    void int_handler() override;
    const char* keycode_name(keycode_t key) override;
    void get_event(keyboard_event_t *output) override;
};
