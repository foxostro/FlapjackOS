#pragma once

template<typename T> struct vec2 {
    T x, y;

    // Equality operator.
    bool operator==(const vec2 &other) const
    {
        if (this == &other) {
            return true;
        }

        return (x == other.x) && (y == other.y);
    }

    // Not-equality operator.
    bool operator!=(const vec2 &other) const
    {
        return !(*this == other);
    }
};

template<typename T> struct size2 {
    T width, height;

    // Equality operator.
    bool operator==(const size2 &other) const
    {
        if (this == &other) {
            return true;
        }

        return (width == other.width) && (height == other.height);
    }

    // Not-equality operator.
    bool operator!=(const size2 &other) const
    {
        return !(*this == other);
    }
};

using point2_t = vec2<int>;
using size2_t = size2<int>;
