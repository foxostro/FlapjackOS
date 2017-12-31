#include "catch.hpp"

#include <common/ring_buffer.hpp>
#include <common/keyboard_device.hpp>
#include <cstdio>

#define VERBOSE 0

TEST_CASE("Initialize an empty buffer", "[ring_buffer]")
{
    constexpr size_t N = 10;
    ring_buffer<int, N> buffer;
    REQUIRE(buffer.size() == 0);
    REQUIRE(buffer.empty());
    REQUIRE(buffer.capacity() == N);
}

static int count_ctor = 0, count_ctor_move = 0, count_ctor_copy = 0, count_dtor = 0;

class whatever {
    int value;

public:

    ~whatever()
    {
        value = -2; // This value lets us see that we're accessing a destructed object.
        count_dtor++;
    }

    whatever(int x) : value(x)
    {
        count_ctor++;
    }

    whatever(whatever &&other) : value(other.value)
    {
        other.value = -1; // This value lets us see that we're accessing a moved-from object.
        count_ctor_move++;
    }

    whatever(const whatever &other) : value(other.value)
    {
        count_ctor_copy++;
    }

    operator int() const
    {
        return value;
    }
};

#if VERBOSE
template<typename T>
static void print_buffer(const char *tag, const T &buffer)
{
    ::printf("%s\t-->\t[ ", tag);
    for (size_t i = 0; i < buffer.size(); ++i) {
        ::printf("%d%s",
                 (int)buffer[i],
                 (i < (buffer.size()-1)) ? ", " : " ");
    }
    ::printf("]\n");
    ::printf("front=%d, back=%d\n", (int)buffer.get_front_pos(), (int)buffer.get_back_pos());
}
#else
template<typename T>
static void print_buffer(const char *, const T &) {}
#endif

TEST_CASE("Push back, pop front", "[ring_buffer]")
{
    {
        ring_buffer<whatever, 3> buffer;

        REQUIRE(buffer.push_back(0));
        print_buffer("buffer.push_back(0)", buffer);
        // [ 0 ]
        REQUIRE(buffer.front() == 0);
        REQUIRE(buffer.back() == 0);

        REQUIRE(buffer.push_back(1));
        print_buffer("buffer.push_back(1)", buffer);
        // [ 0, 1 ]
        REQUIRE(buffer.front() == 0);
        REQUIRE(buffer.back() == 1);

        REQUIRE(buffer.push_back(2));
        print_buffer("buffer.push_back(2)", buffer);
        // [ 0, 1, 2 ]
        REQUIRE(buffer.front() == 0);
        REQUIRE(buffer.back() == 2);

        REQUIRE(!buffer.push_back(3));
        REQUIRE(buffer.full());
        REQUIRE(buffer.size() == buffer.capacity());

        buffer.pop_front();
        print_buffer("buffer.pop_front()", buffer);
        // [ 1, 2 ]
        REQUIRE(buffer.front() == 1);
        REQUIRE(buffer.back() == 2);

        buffer.pop_front();
        print_buffer("buffer.pop_front()", buffer);
        // [ 2 ]
        REQUIRE(buffer.front() == 2);
        REQUIRE(buffer.back() == 2);

        buffer.pop_front();
        print_buffer("buffer.pop_front()", buffer);
        // [ ]
        REQUIRE(buffer.empty());
        REQUIRE(buffer.size() == 0);
    }

    // Make sure destructors get called for each element.
    REQUIRE((count_ctor + count_ctor_move + count_ctor_copy) == count_dtor);
}

TEST_CASE("Push front, pop back", "[ring_buffer]")
{
    ring_buffer<whatever, 3> buffer;

    REQUIRE(buffer.push_front(0));
    print_buffer("buffer.push_front(0)", buffer);
    // [ 0 ]
    REQUIRE(buffer.front() == 0);
    REQUIRE(buffer.back() == 0);

    REQUIRE(buffer.push_front(1));
    print_buffer("buffer.push_front(1)", buffer);
    // [ 0, 1 ]
    REQUIRE(buffer.front() == 1);
    REQUIRE(buffer.back() == 0);

    REQUIRE(buffer.push_front(2));
    print_buffer("buffer.push_front(2)", buffer);
    // [ 0, 1, 2 ]
    REQUIRE(buffer.front() == 2);
    REQUIRE(buffer.back() == 0);

    REQUIRE(!buffer.push_front(3));
    REQUIRE(buffer.full());
    REQUIRE(buffer.size() == buffer.capacity());

    buffer.pop_back();
    print_buffer("buffer.pop_back()", buffer);
    // [ 1, 2 ]
    REQUIRE(buffer.front() == 2);
    REQUIRE(buffer.back() == 1);

    buffer.pop_back();
    print_buffer("buffer.pop_back()", buffer);
    // [ 2 ]
    REQUIRE(buffer.front() == 2);
    REQUIRE(buffer.back() == 2);

    buffer.pop_back();
    print_buffer("buffer.pop_back()", buffer);
    // [ ]
    REQUIRE(buffer.empty());
    REQUIRE(buffer.size() == 0);
}

TEST_CASE("repeated", "[ring_buffer]")
{
    ring_buffer<whatever, 10> buffer;

    // Push front, pop back
    for (int i = 0; i < 2; ++i) {
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.push_front(0));
        print_buffer("buffer.push_front(0)", buffer);
        REQUIRE(buffer.size() == 1);
        REQUIRE(buffer.front() == 0);
        REQUIRE(buffer.back() == 0);
        buffer.pop_back();
        print_buffer("buffer.pop_back()", buffer);
        REQUIRE(buffer.size() == 0);
    }

    // Push back, pop front
    for (int i = 0; i < 2; ++i) {
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.push_back(0));
        print_buffer("buffer.push_back(0)", buffer);
        REQUIRE(buffer.size() == 1);
        REQUIRE(buffer.front() == 0);
        REQUIRE(buffer.back() == 0);
        buffer.pop_front();
        print_buffer("buffer.pop_front()", buffer);
        REQUIRE(buffer.size() == 0);
    }

    // Push front, pop front
    for (int i = 0; i < 2; ++i) {
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.push_front(0));
        print_buffer("buffer.push_front(0)", buffer);
        REQUIRE(buffer.size() == 1);
        REQUIRE(buffer.front() == 0);
        REQUIRE(buffer.back() == 0);
        buffer.pop_front();
        print_buffer("buffer.pop_front()", buffer);
        REQUIRE(buffer.size() == 0);
    }

    // Push back, pop back
    for (int i = 0; i < 2; ++i) {
        REQUIRE(buffer.size() == 0);
        REQUIRE(buffer.push_back(0));
        print_buffer("buffer.push_back(0)", buffer);
        REQUIRE(buffer.size() == 1);
        REQUIRE(buffer.front() == 0);
        REQUIRE(buffer.back() == 0);
        buffer.pop_back();
        print_buffer("buffer.pop_back()", buffer);
        REQUIRE(buffer.size() == 0);
    }
}
