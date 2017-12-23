#pragma once

#include <common/keyboard_device.hpp>
#include <cstddef>

template<typename TYPE, size_t BUFFER_SIZE>
class ring_buffer {
    TYPE buffer[BUFFER_SIZE];
    size_t dequeue_pos;
    size_t enqueue_pos;
    size_t count;
    
public:
    bool enqueue(TYPE event)
    {
        // Drop it on the floor if the buffer is full.
        if (count >= BUFFER_SIZE) {
            return false;
        }

        buffer[enqueue_pos] = event;
        enqueue_pos++;

        if (enqueue_pos >= BUFFER_SIZE) {
            enqueue_pos = 0;
        }

        count++;

        return true;
    }

    // TOOD: This would be an excellent place to use an optional.
    bool dequeue(TYPE &event)
    {
        if(count == 0) {
            return false;
        }

        count--;

        event = buffer[dequeue_pos]; // TODO: Use something like std::move() here?
        dequeue_pos++;

        if(dequeue_pos >= BUFFER_SIZE) {
            dequeue_pos = 0;
        }

        return true;
    }

    ring_buffer() : dequeue_pos(0), enqueue_pos(0), count(0) {}
};

class ps2_keyboard_device : public keyboard_device {
    static constexpr size_t BUFFER_SIZE = 32;

    enum keyboard_driver_state {
        IDLE,
        PROCESSING_ESCAPE_CODE
    };

    keycode_key_state key_state[KEYCODE_MAX];
    ring_buffer<keyboard_event, BUFFER_SIZE> events;

    void clear_input();
    bool step_state_machine(keyboard_driver_state &state, keyboard_event &output); // TODO: Extract this state machine to its own class.

public:
    ps2_keyboard_device();

    void int_handler() override;
    const char* keycode_name(keycode_t key) override;
    keyboard_event get_event() override;
};
