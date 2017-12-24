#pragma once

#include <cstddef>
#include <cstdint>
#include <cstring>

class byte_buffer {
    char *_data;
    size_t _size;

public:
    ~byte_buffer()
    {
        delete[] _data;
    }

    // Default constructor. Initializes an empty buffer.
    byte_buffer()
     : _data(nullptr), _size(0)
    {}

    // Allocates memory for a buffer of the specified size.
    byte_buffer(size_t size)
     : _data(nullptr), _size(size)
    {
        _data = new char[size];
    }

    // Copies the provided buffer.
    byte_buffer(size_t size, const char *src)
     : _data(nullptr), _size(size)
    {
        _data = new char[size];
        memcpy(_data, src, size);
    }

    // Copies the provided string.
    byte_buffer(const char *src)
     : byte_buffer(strlen(src), src)
    {}

    // Copy constructor.
    byte_buffer(const byte_buffer &other)
     : byte_buffer(other._size, other._data)
    {}

    // Move constructor.
    byte_buffer(byte_buffer &&other)
     : _data(other._data), _size(other._size)
    {
        other._data = nullptr;
        other._size = 0;
    }

    // Copy-assignment operator.
    byte_buffer& operator=(const byte_buffer &other)
    {
        delete[] _data;

        _data = new char[_size];
        _size = other._size;
        memcpy(_data, other._data, _size);

        return *this;
    }

    // Move-assignment operator.
    byte_buffer& operator=(byte_buffer &&other)
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
