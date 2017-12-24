#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <utility> // for std::move()

template<typename Type>
class vector {
    static constexpr unsigned GROWTH_FACTOR = 2;

    Type *_data;
    size_t _size, _capacity;

    void increase_capacity_for_insertion()
    {
        if (_size == 0) {
            reserve(1);
        } else if (_size == _capacity) {
            reserve(_size * GROWTH_FACTOR);
        }
    }

public:
    ~vector()
    {
        if (_data) {
            delete[] _data;
        }
    }

    // Default constructor. Initializes an empty vector.
    vector()
     : _data(nullptr), _size(0), _capacity(0)
    {}

    // Allocates memory for a vector of the specified size.
    vector(size_t size)
     : _data(nullptr), _size(size), _capacity(size)
    {
        _data = new Type[_capacity];
    }

    // Copies the provided vector.
    vector(size_t size, const Type *src)
     : _data(nullptr), _size(size), _capacity(size)
    {
        _data = new Type[_capacity];
        for (size_t i = 0; i < _size; ++i) {
            _data[i] = src[i];
        }
    }

    // Copy constructor.
    vector(const vector &other)
     : vector(other._size, other._data)
    {}

    // Move constructor.
    vector(vector &&other)
     : _data(other._data),
       _size(other._size),
       _capacity(other._capacity)
    {
        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;
    }

    // Copy-assignment operator.
    vector& operator=(const vector &other)
    {
        if (_data) {
            delete[] _data;
        }

        _size = other._size;
        _capacity = _size;
        _data = new Type[_capacity];

        for (size_t i = 0; i < _size; ++i) {
            _data[i] = other._data[i];
        }

        return *this;
    }

    // Move-assignment operator.
    vector& operator=(vector &&other)
    {
        if (_data) {
            delete[] _data;
        }

        _data = other._data;
        _size = other._size;
        _capacity = other._capacity;

        other._data = nullptr;
        other._size = 0;
        other._capacity = 0;

        return *this;
    }

    Type& at(size_t i)
    {
        assert(i < _size);
        return _data[i];
    }

    Type at(size_t i) const
    {
        assert(i < _size);
        return _data[i];
    }

    Type& operator[](size_t i)
    {
        return at(i);
    }

    Type operator[](size_t i) const
    {
        return at(i);
    }

    const Type* data() const
    {
        return _data;
    }

    Type* data()
    {
        return _data;
    }

    size_t size() const
    {
        return _size;
    }

    bool empty() const
    {
        return _size == 0;
    }

    size_t capacity() const
    {
        return _capacity;
    }

    void reserve(size_t new_capacity)
    {
        assert(new_capacity >= _capacity); // TODO: shrinking the vector

        Type *new_data = new Type[new_capacity];

        for (size_t i = 0; i < _size; ++i) {
            new_data[i] = std::move(_data[i]);
        }

        delete[] _data;

        _data = new_data;
        _capacity = new_capacity;
    }

    void push_back(Type value)
    {
        increase_capacity_for_insertion();
        _data[_size++] = value;
    }

    void push_front(Type value)
    {
        increase_capacity_for_insertion();

        // Make room at the front.
        for (size_t i = _size; i > 0; --i) {
            _data[i] = std::move(_data[i-1]);
        }

        _data[0] = value;
        _size++;
    }
};
