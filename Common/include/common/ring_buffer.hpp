#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_RING_BUFFER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_RING_BUFFER_HPP

#include <cstddef>
#include <cstdint>
#include <cstring> // for memset
#include <cassert>
#include <utility> // for std::move
#include <type_traits> // for std::aligned_storage
#include <new> // for placement-new

// Statically allocated ring buffer with a fixed capacity.
//
// This is a double-ended queue which can be expanded and contracted at both
// the front or the back ends. It also provides API for random access and for
// insert/delete at arbitrary positions in the queue.
//
// RingBuffer is not a vector-like or array-like container because elements are
// not guaranteed to be contiguous in memory. However, you can often use it like
// a vector that happens to be statically allocated.
template<typename TYPE, int CAPACITY>
class RingBuffer {
public:
    using value_type = TYPE;
    using size_type = int;

private:
    static_assert(CAPACITY > 0, "CAPACITY must be greater than zero.");
    using internal_type = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    // An index into the buffer which wraps around at CAPACITY.
    // This can be incremented, decremented, and cast to size_type.
    class CircularIndex {
        size_type value_;

    public:
        CircularIndex(size_type value) : value_(value) {}
        CircularIndex(const CircularIndex &other) : value_(other.value_) {}

        operator size_type() const
        {
            return value_;
        }

        bool operator==(const CircularIndex &other) const
        {
            if (this == &other) {
                return true;
            }

            return (other.value_ == value_);
        }

        bool operator!=(const CircularIndex &other) const
        {
            return !(*this == other);
        }

        CircularIndex& operator=(const CircularIndex &other)
        {
            if (this != &other) {
                value_ = other.value_;
            }

            return *this;
        }

        CircularIndex& operator++()
        {
            value_ = (value_ + 1) % CAPACITY;
            return *this;
        }

        CircularIndex operator++(int)
        {
            value_ = (value_ + 1) % CAPACITY;
            return *this;
        }

        CircularIndex& operator--()
        {
            if (value_ == 0) {
                value_ = CAPACITY - 1;
            } else {
                value_ = (value_ - 1) % CAPACITY;
            }
            return *this;
        }

        CircularIndex operator--(int)
        {
            if (value_ == 0) {
                value_ = CAPACITY - 1;
            } else {
                value_ = (value_ - 1) % CAPACITY;
            }
            return *this;
        }
    };

    CircularIndex index_at(size_type index) const
    {
        assert(index >= 0 && index <= count_);
        CircularIndex cursor(front_pos_);
        for (size_type i = 0; i < index; ++i) {
            ++cursor;
        }
        return cursor;
    }

    internal_type buffer_[CAPACITY];
    CircularIndex front_pos_;
    CircularIndex back_pos_;
    size_type count_;
    
public:
    ~RingBuffer()
    {
        clear();
    }

    // Default constructor.
    RingBuffer() : front_pos_(0), back_pos_(0), count_(0) {}

    // Copy constructor.
    RingBuffer(const RingBuffer &other)
     : RingBuffer()
    {
        for (size_type i = 0; i < other.size(); ++i) {
            push_back(other.at(i));
        }
    }

    // Move constructor.
    RingBuffer(RingBuffer &&other)
     : RingBuffer()
    {
        for (size_type i = 0; i < other.size(); ++i) {
            push_back(std::move(other.at(i)));
        }
        other.clear();
    }

    // Copy-assignment operator.
    RingBuffer& operator=(const RingBuffer &other)
    {
        if (this != &other) {
            clear();
            for (size_type i = 0; i < other.size(); ++i) {
                push_back(other.at(i));
            }
        }

        return *this;
    }

    // Move-assignment operator.
    RingBuffer& operator=(RingBuffer &&other)
    {
        if (this != &other) {
            clear();
            for (size_type i = 0; i < other.size(); ++i) {
                push_back(std::move(other.at(i)));
            }
            other.clear();
        }

        return *this;
    }

    // Equality operator.
    bool operator==(const RingBuffer &other) const
    {
        if (this == &other) {
            return true;
        }

        if (other.size() != size()) {
            return false;
        }

        for (size_type i = 0, n = size(); i < n; ++i) {
            if (other.at(i) != at(i)) {
                return false;
            }
        }

        return true;
    }

    // Not-equality operator.
    bool operator!=(const RingBuffer &other) const
    {
        return !(*this == other);
    }

    // Returns the element at the front of the buffer.
    value_type& front()
    {
        assert(!empty());
        return *((value_type *)buffer_ + front_pos_);
    }

    // Returns the element at the front of the buffer.
    const value_type& front() const
    {
        assert(!empty());
        return *((value_type *)buffer_ + front_pos_);
    }

    // Returns the element at the back of the buffer.
    value_type& back()
    {
        assert(!empty());
        return *((value_type *)buffer_ + back_pos_);
    }

    // Returns the element at the back of the buffer.
    const value_type& back() const
    {
        assert(!empty());
        return *((value_type *)buffer_ + back_pos_);
    }

    // Returns the i'th element of the RingBuffer.
    // The elements are indexed with zero being the front cursor.
    value_type& at(size_type index)
    {
        CircularIndex cursor = index_at(index);
        return *((value_type *)buffer_ + cursor);
    }

    // Returns the i'th element of the RingBuffer.
    // The elements are indexed with zero being the front cursor.
    const value_type& at(size_type index) const
    {
        CircularIndex cursor = index_at(index);
        return *((value_type *)buffer_ + cursor);
    }

    // Returns the i'th element of the RingBuffer.
    // The elements are indexed with zero being the front cursor.
    value_type& operator[](size_type i)
    {
        return at(i);
    }

    // Returns the i'th element of the RingBuffer.
    // The elements are indexed with zero being the front cursor.
    const value_type& operator[](size_type i) const
    {
        return at(i);
    }

    // Removes all elements from the RingBuffer.
    void clear()
    {
        while (!empty()) {
            pop_front();
        }
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
            back_pos_++;
        }

        count_++;
        new ((value_type *)buffer_ + back_pos_) value_type(std::move(value));

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
            front_pos_--;
        }

        count_++;
        new ((value_type *)buffer_ + front_pos_) value_type(std::move(value));

        return true;
    }

    // Pops an item off the back of the RingBuffer.
    // This cannot be called if the RingBuffer is empty.
    void pop_back()
    {
        assert(!empty());
        value_type *ptr = (value_type *)buffer_ + back_pos_;
        ptr->~value_type();
        if (size() != 1) {
            back_pos_--;
        }
        count_--;
    }

    // Pops an item off the front of the RingBuffer.
    // This cannot be called if the RingBuffer is empty.
    void pop_front()
    {
        assert(!empty());
        value_type *ptr = (value_type *)buffer_ + front_pos_;
        ptr->~value_type();
        if (size() != 1) {
            front_pos_++;
        }
        count_--;
    }

    // Inserts an item at the specified index of the RingBuffer.
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

        back_pos_++;

        for (size_type i = count_; i > index; --i) {
            value_type *prev = (value_type *)buffer_ + index_at(i-1);
            value_type *curr = (value_type *)buffer_ + index_at(i+0);
            new (curr) value_type(std::move(*prev));
            prev->~value_type();
        }

        new ((value_type *)buffer_ + index_at(index)) value_type(std::move(value));
        count_++;

        return true;
    }

    // Removes the index'th element of the RingBuffer.
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

        for (size_type i = index; i < count_-1; ++i) {
            value_type *curr = (value_type *)buffer_ + index_at(i+0);
            value_type *next = (value_type *)buffer_ + index_at(i+1);
            curr->~value_type();
            new (curr) value_type(std::move(*next));
        }

        if (size() != 1) {
            back_pos_--;
        }

        count_--;
    }

    // Returns true if the RingBuffer is empty.
    bool empty() const
    {
        return count_ == 0;
    }

    // Returns true if the RingBuffer is at capacity.
    bool full() const
    {
        return count_ >= CAPACITY;
    }

    // Returns the number of elements in the RingBuffer;
    size_type size() const
    {
        return count_;
    }

    // Returns the maximum capacity of the RingBuffer;
    size_type capacity() const
    {
        return CAPACITY;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_RING_BUFFER_HPP
