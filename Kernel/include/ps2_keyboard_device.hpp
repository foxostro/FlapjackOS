#pragma once

#include <common/keyboard_device.hpp>
#include <common/ring_buffer.hpp>

class ps2_keyboard_device : public keyboard_device {
    enum keyboard_driver_state {
        IDLE,
        PROCESSING_ESCAPE_CODE
    };

    keycode_key_state key_state[KEYCODE_MAX];
    ring_buffer<keyboard_event, 32> events;

    void clear_input();
    bool step_state_machine(keyboard_driver_state &state, keyboard_event &output); // TODO: Extract this state machine to its own class.

public:
    ps2_keyboard_device();

    void int_handler() override;
    const char* keycode_name(keycode_t key) override;
    keyboard_event get_event() override;
};
