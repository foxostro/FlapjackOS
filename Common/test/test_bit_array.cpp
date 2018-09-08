#include "catch.hpp"

#include <common/bit_array.hpp>
#include <cstdio>

constexpr size_t NUMBER_OF_PAGE_FRAMES = 8175+1;

static BitArray& create_bit_array(size_t number_of_bits)
{
    size_t size = BitArray::calc_number_of_storage_bytes(number_of_bits);
    BitArray* result = new BitArray(number_of_bits, size, new char[size]);
    return *result;
}

TEST_CASE("Initialize an empty bit array", "[BitArray]")
{
    REQUIRE(sizeof(BitArray::Word) == 4);
    REQUIRE(BitArray::NUMBER_OF_WORD_BITS == 32);
    REQUIRE((BitArray::calc_number_of_storage_bytes(NUMBER_OF_PAGE_FRAMES) % sizeof(BitArray::Word)) == 0);
    BitArray &bits = create_bit_array(NUMBER_OF_PAGE_FRAMES);
    REQUIRE(bits.get_number_of_bits() == NUMBER_OF_PAGE_FRAMES);
    for (size_t i = 0; i < bits.get_number_of_bits(); ++i) {
        REQUIRE(bits.test(i) == false);
    }
}

TEST_CASE("Test individual bit manipulation", "[BitArray]")
{
    BitArray &bits = create_bit_array(NUMBER_OF_PAGE_FRAMES);
    bits.set(500);
    bits.set(8175);
    for (size_t i = 0; i < bits.get_number_of_bits(); ++i) {
        REQUIRE(bits.test(i) == ((i == 500) || (i == 8175)));
    }
    bits.clear(500);
    for (size_t i = 0; i < bits.get_number_of_bits(); ++i) {
        REQUIRE(bits.test(i) == (i == 8175));
    }
}

TEST_CASE("Test region bit manipulation", "[BitArray]")
{
    BitArray &bits = create_bit_array(NUMBER_OF_PAGE_FRAMES);
    bits.set_region(10, 3);
    for (size_t i = 0; i < bits.get_number_of_bits(); ++i) {
        bool expected = ((i == 10) || (i == 11) || (i == 12));
        REQUIRE(bits.test(i) == expected);
    }
    bits.clear_region(0, 100);
    for (size_t i = 0; i < bits.get_number_of_bits(); ++i) {
        REQUIRE(bits.test(i) == false);
    }
}

TEST_CASE("Test bit scanning", "[BitArray]")
{
    BitArray &bits = create_bit_array(NUMBER_OF_PAGE_FRAMES);
    REQUIRE(bits.scan_forward(true) == BitArray::NOT_FOUND);
    REQUIRE(bits.scan_backward(true) == BitArray::NOT_FOUND);
    bits.set_region(bits.get_number_of_bits()-10, 10);
    REQUIRE(bits.scan_forward(true) == bits.get_number_of_bits()-10);
    REQUIRE(bits.test(bits.get_number_of_bits()-11) == false);
    REQUIRE(bits.scan_backward(false) == bits.get_number_of_bits()-11);
}
