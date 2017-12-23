#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

// Statically allocated ring buffer.
// This is designed to be appropriate for use in an interrupt context.
template<typename TYPE, size_t BUFFER_SIZE>
class ring_buffer {
    static_assert(BUFFER_SIZE>0, "BUFFER_SIZE must be greater than zero");
    TYPE _buffer[BUFFER_SIZE];
    size_t _dequeue_pos;
    size_t _enqueue_pos;
    size_t _count;
    
public:
    // Enqueues the specified value into the ring _buffer.
    // Returns true if this was successful, and false otherwise.
    // This may fail if the ring _buffer is full.
    bool enqueue(TYPE value)
    {
        // Drop it on the floor if the _buffer is full.
        if (full()) {
            return false;
        }

        _buffer[_enqueue_pos] = value;
        _enqueue_pos++;

        if (_enqueue_pos >= BUFFER_SIZE) {
            _enqueue_pos = 0;
        }

        _count++;

        return true;
    }

    // Dequeues and returns the next item in the ring _buffer.
    // This cannot be called if the ring _buffer is empty.
    TYPE dequeue()
    {
        assert(!empty());

        _count--;

        // TODO: Use something like std::move() here?
        TYPE value = _buffer[_dequeue_pos];
        _dequeue_pos++;

        if(_dequeue_pos >= BUFFER_SIZE) {
            _dequeue_pos = 0;
        }

        return value;
    }

    // Returns true if the ring _buffer is empty.
    bool empty() const
    {
        return _count == 0;
    }

    // Returns true if the ring _buffer is at capacity.
    bool full() const
    {
        return _count >= BUFFER_SIZE;
    }

    // Returns the number of elements in the ring _buffer;
    size_t count() const
    {
        return _count;
    }

    // Returns the maximum capacity of the ring _buffer;
    size_t capacity() const
    {
        return BUFFER_SIZE;
    }

    ring_buffer() : _dequeue_pos(0), _enqueue_pos(0), _count(0) {}
};
