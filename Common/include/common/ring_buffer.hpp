#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring> // for memset
#include <cassert>
#include <utility> // for std::move
#include <type_traits> // for std::aligned_storage
#include <new> // for placement-new

// Statically allocated ring buffer with a fixed capacity.
// This is a double-ended queue which can be expanded and contracted at both
// the front or the back ends.
template<typename TYPE, int CAPACITY>
class ring_buffer {
public:
    using value_type = TYPE;
    using size_type = int;

private:
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

    circular_index index_at(size_type index) const
    {
        assert(index >= 0 && index <= _count);
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

    circular_index get_front_pos() const { return _front_pos; }; // TODO: REMOVE ME
    circular_index get_back_pos() const { return _back_pos; };

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
        new ((value_type *)_buffer + _back_pos) value_type(std::move(value));

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
        new ((value_type *)_buffer + _front_pos) value_type(std::move(value));

        return true;
    }

    // Pops an item off the back of the ring_buffer.
    // This cannot be called if the ring_buffer is empty.
    void pop_back()
    {
        assert(!empty());
        value_type *ptr = (value_type *)_buffer + _back_pos;
        ptr->~value_type();
        if (size() != 1) {
            _back_pos--;
        }
        _count--;
    }

    // Pops an item off the front of the ring_buffer.
    // This cannot be called if the ring_buffer is empty.
    void pop_front()
    {
        assert(!empty());
        value_type *ptr = (value_type *)_buffer + _front_pos;
        ptr->~value_type();
        if (size() != 1) {
            _front_pos++;
        }
        _count--;
    }

    // Inserts an item at the specified index of the ring_buffer.
    // Moves item toward the back to make room.
    // Returns true if this was successful, and false otherwise.
    // This may fail if there is not enough space.
    bool insert(size_type index, value_type value)
    {
        assert(index >= 0 && index <= size());

        if (full()) {
            return false;
        }

        if (index == 0) {
            return push_front(std::move(value));
        }

        if (index == size()) {
            return push_back(std::move(value));
        }

        _back_pos++;

        for (size_type i = _count; i > index; --i) {
            value_type *prev = (value_type *)_buffer + index_at(i-1);
            value_type *curr = (value_type *)_buffer + index_at(i+0);
            new (curr) value_type(std::move(*prev));
            prev->~value_type();
        }

        new ((value_type *)_buffer + index_at(index)) value_type(std::move(value));
        _count++;

        return true;
    }

    // Removes the index'th element of the ring_buffer.
    // Moves item from the back toward the front to fill the hole.
    void remove(size_type index)
    {
        assert(!empty());
        assert(index >= 0 && index < size());

        if (index == 0) {
            pop_front();
            return;
        }

        if (index == size()-1) {
            pop_back();
            return;
        }

        for (size_type i = index; i < _count-1; ++i) {
            value_type *curr = (value_type *)_buffer + index_at(i+0);
            value_type *next = (value_type *)_buffer + index_at(i+1);
            curr->~value_type();
            new (curr) value_type(std::move(*next));
        }

        _count--;
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
    size_type size() const
    {
        return _count;
    }

    // Returns the maximum capacity of the ring_buffer;
    size_type capacity() const
    {
        return CAPACITY;
    }
};
