#include "catch.hpp"

#include <common/ring_buffer.hpp>

TEST_CASE("Initialize an empty buffer", "[RingBuffer]")
{
    constexpr size_t N = 10;
    ring_buffer<int, N> buffer;
    REQUIRE(buffer.count() == 0);
    REQUIRE(buffer.empty());
    REQUIRE(buffer.capacity() == N);
}

TEST_CASE("Enqueue until full, dequeue until empty", "[RingBuffer]")
{
    ring_buffer<int, 3> buffer;

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