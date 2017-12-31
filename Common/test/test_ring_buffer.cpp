#include "catch.hpp"

#include <common/ring_buffer.hpp>
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

        REQUIRE(buffer.pop_front() == 0);
        print_buffer("buffer.pop_front()", buffer);
        // [ 1, 2 ]
        REQUIRE(buffer.front() == 1);
        REQUIRE(buffer.back() == 2);

        REQUIRE(buffer.pop_front() == 1);
        print_buffer("buffer.pop_front()", buffer);
        // [ 2 ]
        REQUIRE(buffer.front() == 2);
        REQUIRE(buffer.back() == 2);

        REQUIRE(buffer.pop_front() == 2);
        print_buffer("buffer.pop_front()", buffer);
        // [ ]
        REQUIRE(buffer.empty());
        REQUIRE(buffer.size() == 0);
    }

    // Make sure destructors get called for each element.
    REQUIRE((count_ctor + count_ctor_move + count_ctor_copy) == count_dtor);
}

TEST_CASE("Push front", "[ring_buffer]")
{
    ring_buffer<whatever, 3> buffer;
    REQUIRE(buffer.push_front(0));
    print_buffer("buffer.push_front(0)", buffer);
    // [ 0 ]
    REQUIRE(buffer.size() == 1);
    REQUIRE(buffer.front() == 0);
    REQUIRE(buffer.back() == 0);

    REQUIRE(buffer.push_front(1));
    print_buffer("buffer.push_front(1)", buffer);
    // [ 1, 0 ]
    REQUIRE(buffer.size() == 2);
    REQUIRE(buffer.front() == 1);
    REQUIRE(buffer.back() == 0);

    REQUIRE(buffer.push_front(2));
    print_buffer("buffer.push_front(2)", buffer);
    // [ 2, 1, 0 ]
    REQUIRE(buffer.size() == 3);
    REQUIRE(buffer.front() == 2);
    REQUIRE(buffer.back() == 0);
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

    REQUIRE(buffer.pop_back() == 0);
    print_buffer("buffer.pop_back()", buffer);
    // [ 1, 2 ]
    REQUIRE(buffer.front() == 2);
    REQUIRE(buffer.back() == 1);

    REQUIRE(buffer.pop_back() == 1);
    print_buffer("buffer.pop_back()", buffer);
    // [ 2 ]
    REQUIRE(buffer.front() == 2);
    REQUIRE(buffer.back() == 2);

    REQUIRE(buffer.pop_back() == 2);
    print_buffer("buffer.pop_back()", buffer);
    // [ ]
    REQUIRE(buffer.empty());
    REQUIRE(buffer.size() == 0);
}
