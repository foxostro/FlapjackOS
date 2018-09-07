#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_VEC2_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_VEC2_HPP

template<typename T> struct Vec2_ {
    T x, y;

    // Equality operator.
    bool operator==(const Vec2_ &other) const
    {
        if (this == &other) {
            return true;
        }

        return (x == other.x) && (y == other.y);
    }

    // Not-equality operator.
    bool operator!=(const Vec2_ &other) const
    {
        return !(*this == other);
    }
};

template<typename T> struct Size2_ {
    T width, height;

    // Equality operator.
    bool operator==(const Size2_ &other) const
    {
        if (this == &other) {
            return true;
        }

        return (width == other.width) && (height == other.height);
    }

    // Not-equality operator.
    bool operator!=(const Size2_ &other) const
    {
        return !(*this == other);
    }
};

using Point2 = Vec2_<int>;
using Size2 = Size2_<int>;

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_VEC2_HPP
