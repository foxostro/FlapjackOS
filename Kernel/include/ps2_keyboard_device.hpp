#pragma once

#include <interfaces/keyboard_device.hpp>
#include <stddef.h>

class ps2_keyboard_device : public keyboard_device {
    static constexpr size_t BUFFER_SIZE = 32;

    enum keyboard_driver_state {
        IDLE,
        PROCESSING_ESCAPE_CODE
    };

    keycode_key_state key_state[KEYCODE_MAX];
    keyboard_event ring_buffer[BUFFER_SIZE]; // TODO: Extract this ring buffer to its own class.
    size_t dequeue_pos;
    size_t enqueue_pos;
    size_t count;

    void enqueue(const keyboard_event &event);
    bool dequeue(keyboard_event &event); // TOOD: This is an excellent place for an optional.
    void clear_input();
    bool step_state_machine(keyboard_driver_state &state, keyboard_event &output); // TODO: Extract this state machine to its own class.

public:
    ps2_keyboard_device();

    void int_handler() override;
    const char* keycode_name(keycode_t key) override;
    keyboard_event get_event() override;
};
