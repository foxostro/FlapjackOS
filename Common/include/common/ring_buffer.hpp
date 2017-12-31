#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>
#include <utility> // for std::move()
#include <type_traits> // for std::aligned_storage
#include <new> // for placement-new

// Statically allocated ring buffer.
// This is designed to be appropriate for use in an interrupt context.
template<typename TYPE, int CAPACITY>
class ring_buffer {
    using value_type = TYPE;
    using size_type = int;

    static_assert(CAPACITY > 0, "CAPACITY must be greater than zero.");
    using internal_type = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    internal_type _buffer[CAPACITY];
    size_type _dequeue_pos;
    size_type _enqueue_pos;
    size_type _count;
    
public:
    ~ring_buffer()
    {
        while (!empty()) {
            pop_front();
        }
    }

    ring_buffer() : _dequeue_pos(0), _enqueue_pos(0), _count(0) {}

    // Enqueues the specified value into the ring_buffer.
    // Returns true if this was successful, and false otherwise.
    // This may fail if the ring_buffer is full.
    bool push_back(value_type value)
    {
        // Drop it on the floor if the _buffer is full.
        if (full()) {
            return false;
        }

        new ((value_type *)_buffer + _enqueue_pos) value_type(value);
        _enqueue_pos++;

        if (_enqueue_pos >= CAPACITY) {
            _enqueue_pos = 0;
        }

        _count++;

        return true;
    }

    // Dequeues and returns the next item in the ring_buffer.
    // This cannot be called if the ring_buffer is empty.
    value_type pop_front()
    {
        assert(!empty());

        _count--;

        value_type *ptr = (value_type *)_buffer + _dequeue_pos;
        value_type value = std::move(*ptr);
        ptr->~value_type();
        _dequeue_pos++;

        if(_dequeue_pos >= CAPACITY) {
            _dequeue_pos = 0;
        }

        return value;
    }

    // Returns true if the ring_buffer is empty.
    bool empty() const
    {
        return _count == 0;
    }

    // Returns true if the ring_buffer is at capacity.
    bool full() const
    {
        return _count >= CAPACITY;
    }

    // Returns the number of elements in the ring_buffer;
    size_t size() const
    {
        return _count;
    }

    // Returns the maximum capacity of the ring_buffer;
    size_t capacity() const
    {
        return CAPACITY;
    }
};
