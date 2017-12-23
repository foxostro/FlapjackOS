#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

// Statically allocated ring buffer collection.
// This is designed to be appropriate for use in an interrupt context.
template<typename TYPE, size_t BUFFER_SIZE>
class ring_buffer {
    TYPE buffer[BUFFER_SIZE];
    size_t dequeue_pos;
    size_t enqueue_pos;
    size_t count;
    
public:
    // Enqueues the specified value into the ring buffer.
    // Returns true if this was successful, and false otherwise.
    // This may fail if the ring buffer is full.
    bool enqueue(TYPE value)
    {
        // Drop it on the floor if the buffer is full.
        if (full()) {
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

    // Dequeues and returns the next item in the ring buffer.
    // This cannot be called if the ring buffer is empty.
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

    // Returns true if the ring buffer is empty.
    bool empty() const
    {
        return count == 0;
    }

    // Returns true if the ring buffer is at capacity.
    bool full() const
    {
        return count >= BUFFER_SIZE;
    }

    ring_buffer() : dequeue_pos(0), enqueue_pos(0), count(0) {}
};
