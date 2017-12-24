#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <utility> // for std::move()

template<typename Type>
class vector {
    Type *_data;
    size_t _size;

public:
    ~vector()
    {
        delete[] _data;
    }

    // Default constructor. Initializes an empty buffer.
    vector()
     : _data(nullptr), _size(0)
    {}

    // Allocates memory for a buffer of the specified size.
    vector(size_t size)
     : _data(nullptr), _size(size)
    {
        _data = new char[size];
    }

    // Copies the provided buffer.
    vector(size_t size, const char *src)
     : _data(nullptr), _size(size)
    {
        _data = new char[size];
        memcpy(_data, src, size);
    }

    // Copies the provided string.
    vector(const char *src)
     : vector(strlen(src), src)
    {}

    // Copy constructor.
    vector(const vector &other)
     : vector(other._size, other._data)
    {}

    // Move constructor.
    vector(vector &&other)
     : _data(other._data), _size(other._size)
    {
        other._data = nullptr;
        other._size = 0;
    }

    // Copy-assignment operator.
    vector& operator=(const vector &other)
    {
        delete[] _data;

        _data = new char[_size];
        _size = other._size;

        for (size_t i = 0; i < _size; ++i) {
            _data[i] = std::move(other._data[i]);
        }

        return *this;
    }

    // Move-assignment operator.
    vector& operator=(vector &&other)
    {
        delete[] _data;

        _data = other._data;
        _size = other._size;

        other._data = nullptr;
        other._size = 0;

        return *this;
    }

    const char* data() const
    {
        return _data;
    }

    char* data()
    {
        return _data;
    }

    char operator[](size_t i) const
    {
        assert(i < _size);
        return _data[i];
    }

    char& operator[](size_t i)
    {
        assert(i < _size);
        return _data[i];
    }

    size_t size() const
    {
        return _size;
    }
};
