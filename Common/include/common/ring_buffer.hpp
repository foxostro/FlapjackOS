#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring> // for memset
#include <cassert>
#include <utility> // for std::move
#include <type_traits> // for std::aligned_storage
#include <new> // for placement-new

// Statically allocated ring buffer.
// This is designed to be appropriate for use in an interrupt context.
template<typename TYPE, int CAPACITY>
class ring_buffer {
    using value_type = TYPE;
    using size_type = size_t;

    static_assert(CAPACITY > 0, "CAPACITY must be greater than zero.");
    using internal_type = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    // An index into the buffer which wraps around at CAPACITY.
    // This can be incremented, decremented, and cast to size_type.
    class circular_index {
        size_type _value;

    public:
        circular_index(size_type value) : _value(value) {}
        circular_index(const circular_index &other) : _value(other._value) {}

        operator size_type() const
        {
            return _value;
        }

        bool operator==(const circular_index &other) const
        {
            if (this == &other) {
                return true;
            }

            return (other._value == _value);
        }

        bool operator!=(const circular_index &other) const
        {
            return !(*this == other);
        }

        circular_index& operator=(const circular_index &other)
        {
            if (this != &other) {
                _value = other._value;
            }

            return *this;
        }

        circular_index& operator++()
        {
            _value = (_value + 1) % CAPACITY;
            return *this;
        }

        circular_index operator++(int)
        {
            _value = (_value + 1) % CAPACITY;
            return *this;
        }

        circular_index& operator--()
        {
            if (_value == 0) {
                _value = CAPACITY - 1;
            } else {
                _value = (_value - 1) % CAPACITY;
            }
            return *this;
        }

        circular_index operator--(int)
        {
            if (_value == 0) {
                _value = CAPACITY - 1;
            } else {
                _value = (_value - 1) % CAPACITY;
            }
            return *this;
        }
    };

    circular_index index_at(size_type index)
    {
        assert(index >= 0 && index < _count);
        circular_index cursor(_front_pos);
        for (size_type i = 0; i < index; ++i) {
            ++cursor;
        }
        return cursor;
    }

    internal_type _buffer[CAPACITY];
    circular_index _front_pos;
    circular_index _back_pos;
    size_type _count;
    
public:
    ~ring_buffer()
    {
        while (!empty()) {
            pop_front();
        }
    }

    ring_buffer() : _front_pos(0), _back_pos(0), _count(0) {}

    // Returns the element at the front of the buffer.
    value_type& front()
    {
        assert(!empty());
        return *((value_type *)_buffer + _front_pos);
    }

    // Returns the element at the front of the buffer.
    const value_type& front() const
    {
        assert(!empty());
        return *((value_type *)_buffer + _front_pos);
    }

    // Returns the element at the back of the buffer.
    value_type& back()
    {
        assert(!empty());
        return *((value_type *)_buffer + _back_pos);
    }

    // Returns the element at the back of the buffer.
    const value_type& back() const
    {
        assert(!empty());
        return *((value_type *)_buffer + _back_pos);
    }

    // Returns the i'th element of the ring_buffer.
    // The elements are indexed with zero being the front cursor.
    value_type& at(size_type index)
    {
        circular_index cursor = index_at(index);
        return *((value_type *)_buffer + cursor);
    }

    // Returns the i'th element of the ring_buffer.
    // The elements are indexed with zero being the front cursor.
    const value_type& at(size_type index) const
    {
        circular_index cursor = index_at(index);
        return *((value_type *)_buffer + cursor);
    }

    // Returns the i'th element of the ring_buffer.
    // The elements are indexed with zero being the front cursor.
    value_type& operator[](size_type i)
    {
        return at(i);
    }

    // Returns the i'th element of the ring_buffer.
    // The elements are indexed with zero being the front cursor.
    const value_type& operator[](size_type i) const
    {
        return at(i);
    }

    // Pushes an item onto the back of the buffer.
    // Returns true if this was successful, and false otherwise.
    // This may fail if there is not enough space.
    bool push_back(value_type value)
    {
        // Drop it on the floor if the buffer is full.
        if (full()) {
            return false;
        }

        if (!empty()) {
            _back_pos++;
        }

        _count++;
        new ((value_type *)_buffer + _back_pos) value_type(value);

        return true;
    }

    // Pushes an item onto the front of the buffer.
    // Returns true if this was successful, and false otherwise.
    // This may fail if there is not enough space.
    bool push_front(value_type value)
    {
        // Drop it on the floor if the buffer is full.
        if (full()) {
            return false;
        }

        if (!empty()) {
            _front_pos--;
        }

        _count++;
        new ((value_type *)_buffer + _front_pos) value_type(value);

        return true;
    }

    // Pops an item off the back of the ring_buffer, and returns it.
    // This cannot be called if the ring_buffer is empty.
    value_type pop_back()
    {
        assert(!empty());

        value_type *ptr = (value_type *)_buffer + _back_pos;
        value_type value = std::move(*ptr);
        ptr->~value_type();
        _back_pos--;
        _count--;

        return value;
    }

    // Pops an item off the front of the ring_buffer, and returns it.
    // This cannot be called if the ring_buffer is empty.
    value_type pop_front()
    {
        assert(!empty());

        value_type *ptr = (value_type *)_buffer + _front_pos;
        value_type value = std::move(*ptr);
        ptr->~value_type();
        _front_pos++;
        _count--;

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
