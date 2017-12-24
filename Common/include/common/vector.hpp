#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <utility> // for std::move()
#include <type_traits> // for std::aligned_storage
#include <new> // for placement-new

template<typename T>
class vector {
public:
    using value_type = T;
    using size_type = int;
    using internal_type = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    ~vector()
    {
        if (_data) {
            delete[] _data;
        }
    }

    // Default constructor. Initializes an empty vector.
    vector()
     : _data(nullptr), _count(0), _capacity(0)
    {}

    // Allocates memory for a vector of the specified size.
    vector(size_type size)
     : _data(new internal_type[size]), _count(size), _capacity(size)
    {
        for (size_type i = 0; i < _count; ++i) {
            new (data() + i) value_type();
        }
    }

    // Copies the provided items into the vector.
    vector(size_type size, const value_type *src)
     : _data(nullptr), _count(size), _capacity(size)
    {
        _data = new internal_type[_capacity];
        for (size_type i = 0; i < _count; ++i) {
            (value_type &)(_data[i]) = src[i];
        }
    }

    // Copy constructor.
    vector(const vector &other)
     : vector(other._count, other.data())
    {}

    // Move constructor.
    vector(vector &&other)
     : _data(other._data),
       _count(other._count),
       _capacity(other._capacity)
    {
        other._data = nullptr;
        other._count = 0;
        other._capacity = 0;
    }

    // Copy-assignment operator.
    vector& operator=(const vector &other)
    {
        if (this != &other) {
            if (_data) {
                delete[] _data;
            }

            _count = other._count;
            _capacity = _count;
            _data = new internal_type[_capacity];

            for (size_type i = 0; i < _count; ++i) {
                (value_type &)(_data[i]) = (value_type &)(other._data[i]);
            }
        }

        return *this;
    }

    // Move-assignment operator.
    vector& operator=(vector &&other)
    {
        if (this != &other) {
            if (_data) {
                delete[] _data;
            }

            _data = other._data;
            _count = other._count;
            _capacity = other._capacity;

            other._data = nullptr;
            other._count = 0;
            other._capacity = 0;
        }

        return *this;
    }

    // Equality operator.
    bool operator==(const vector &other) const
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
    bool operator!=(const vector &other) const
    {
        return !(*this == other);
    }

    // Returns the address of the first element in memory.
    // The vector gaurantees that all elements are contiguous in memory.
    const value_type* data() const
    {
        return (value_type *)_data;
    }

    // Returns the address of the first element in memory.
    // The vector gaurantees that all elements are contiguous in memory.
    value_type* data()
    {
        return (value_type *)_data;
    }

    // Returns the number of items in the collection.
    size_type size() const
    {
        return _count;
    }

    // Returns true if the collection is empty.
    bool empty() const
    {
        return _count == 0;
    }

    // Returns the number of elements for which space is currently allocated.
    size_type capacity() const
    {
        return _capacity;
    }

    // Gets the index'th element of the collection.
    value_type& at(size_type i)
    {
        assert(i >= 0 && i < _count);
        return data()[i];
    }

    // Gets the index'th element of the collection.
    value_type& operator[](size_type i)
    {
        return at(i);
    }

    // Gets the index'th element of the collection.
    value_type at(size_type i) const
    {
        assert(i >= 0 && i < _count);
        return data()[i];
    }

    // Gets the index'th element of the collection.
    value_type operator[](size_type i) const
    {
        return at(i);
    }

    // Increases the capacity of the vector such that at least the specified
    // number of elements may be inserted before additional allocations occur.
    void reserve(size_type new_capacity)
    {
        assert(new_capacity >= 0);

        if(new_capacity > _capacity) {
            internal_type *new_data = new internal_type[new_capacity];

            for (size_type i = 0; i < _count; ++i) {
                ((value_type &)new_data[i]) = std::move(data()[i]);
            }

            delete[] _data;
            _data = new_data;
            _capacity = new_capacity;
        }
    }

    // Inserts an item at the end of the collection.
    void push_back(value_type value)
    {
        insert(_count, value);
    }

    // Inserts an item at the beginning of the collection.
    // All existing items are moved to make room at the beginning.
    void push_front(value_type value)
    {
        insert(0, value);
    }

    // Inserts an item at the specified index of the collection.
    // All existing items are moved toward the end to make room.
    void insert(size_type index, value_type value)
    {
        assert(index >= 0 && index <= _count);

        increase_capacity_for_insertion();

        for (size_type i = _count; i > index; --i) {
            value_type &prev = data()[i-1];
            new (data() + i) value_type(std::move(prev));
            prev.~value_type();
        }

        new (data() + index) value_type(value);
        _count++;
    }

    // Removes the index'th element of the collection.
    // Returns the value of the removed element.
    value_type remove(size_type index)
    {
        assert(index >= 0 && index < _count);

        value_type old_value = data()[index];

        for (size_type i = index; i < _count-1; ++i) {
            value_type &curr = data()[i];
            curr.~value_type();

            value_type &next = data()[i+1];
            new (data() + i) value_type(std::move(next));
        }

        _count--;

        return old_value;
    }

    // Removes the last element of the collection.
    // Returns the value of the removed element.
    value_type pop_back()
    {
        return remove(size()-1);
    }

    // Removes the first element of the collection.
    // Returns the value of the removed element.
    value_type pop_front()
    {
        return remove(0);
    }

private:
    static constexpr unsigned GROWTH_FACTOR = 2;

    internal_type *_data;
    size_type _count, _capacity;

    void increase_capacity_for_insertion()
    {
        if (_capacity == 0) {
            reserve(1);
        } else if (_count == _capacity) {
            reserve(_count * GROWTH_FACTOR);
        }
    }
};
