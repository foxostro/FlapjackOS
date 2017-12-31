#include "catch.hpp"

#include <common/ring_buffer.hpp>

TEST_CASE("Initialize an empty buffer", "[ring_buffer]")
{
    constexpr size_t N = 10;
    ring_buffer<int, N> buffer;
    REQUIRE(buffer.count() == 0);
    REQUIRE(buffer.empty());
    REQUIRE(buffer.capacity() == N);
}

static int count_ctor = 0, count_ctor_move = 0, count_ctor_copy = 0, count_dtor = 0;

TEST_CASE("Enqueue until full, dequeue until empty", "[ring_buffer]")
{
    class whatever {
        int value;
        
    public:

        ~whatever()
        {
            count_dtor++;
        }

        whatever(int x) : value(x)
        {
            count_ctor++;
        }

        whatever(whatever &&other) : value(other.value)
        {
            other.value = -1;
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

    {
        ring_buffer<whatever, 3> buffer;

        // Enqueue until full. Dequeue until empty.
        // Do this a few times to ensure we cross the internal buffer's boundary.
        for (size_t i = 0; i < 3; ++i) {
            REQUIRE(buffer.enqueue(0));
            REQUIRE(buffer.enqueue(1));
            REQUIRE(buffer.enqueue(2));
            REQUIRE(!buffer.enqueue(3));
            REQUIRE(buffer.full());
            REQUIRE(buffer.count() == buffer.capacity());

            REQUIRE(buffer.dequeue() == 0);
            REQUIRE(buffer.dequeue() == 1);
            REQUIRE(buffer.dequeue() == 2);
            REQUIRE(buffer.empty());
            REQUIRE(buffer.count() == 0);
        }
    }

    // Make sure destructors get called for each element.
    REQUIRE((count_ctor + count_ctor_move + count_ctor_copy) == count_dtor);
}