#pragma once

#include <common/keyboard_device.hpp>
#include <cstddef>
#include <cassert>

template<typename TYPE, size_t BUFFER_SIZE>
class ring_buffer {
    TYPE buffer[BUFFER_SIZE];
    size_t dequeue_pos;
    size_t enqueue_pos;
    size_t count;
    
public:
    bool enqueue(TYPE value)
    {
        // Drop it on the floor if the buffer is full.
        if (count >= BUFFER_SIZE) {
            return false;
        }

        buffer[enqueue_pos] = value;
        enqueue_pos++;

        if (enqueue_pos >= BUFFER_SIZE) {
            enqueue_pos = 0;
        }

        count++;

        return true;
    }

    bool empty() const
    {
        return count == 0;
    }

    TYPE dequeue()
    {
        assert(!empty());

        count--;

        // TODO: Use something like std::move() here?
        TYPE value = buffer[dequeue_pos];
        dequeue_pos++;

        if(dequeue_pos >= BUFFER_SIZE) {
            dequeue_pos = 0;
        }

        return value;
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
