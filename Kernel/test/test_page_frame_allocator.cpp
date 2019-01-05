#include "catch.hpp"
#include "concrete_page_frame_allocator.hpp"

constexpr size_t CAPACITY = 10;
using TestingAllocator = GenericConcretePageFrameAllocator<CAPACITY>;

TEST_CASE("test_page_frame_allocator_default_ctor", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    REQUIRE(allocator.get_capacity() == CAPACITY);
    REQUIRE(allocator.get_number_of_free_page_frames() == 0);
}

TEST_CASE("test_page_frame_allocator_deallocate", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(0);
    REQUIRE(allocator.is_free(0) == true);
    REQUIRE(allocator.get_number_of_free_page_frames() == 1);
}

TEST_CASE("test_page_frame_allocator_deallocate_free_page_frame", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(4096);
    allocator.deallocate(4096);
    REQUIRE(allocator.is_free(4096) == true);
    REQUIRE(allocator.get_number_of_free_page_frames() == 1);
}

TEST_CASE("test_page_frame_allocator_allocation_fails", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    REQUIRE(allocator.is_free(0) == false);
    REQUIRE(allocator.allocate(0) == false);
}

TEST_CASE("test_page_frame_allocator_allocation_succeeds", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(0);
    REQUIRE(allocator.is_free(0) == true);
    REQUIRE(allocator.allocate(0) == true);
}

TEST_CASE("test_page_frame_allocator_allocate_is_span_free_0", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    REQUIRE(allocator.is_span_free(0, 0) == false);
}

TEST_CASE("test_page_frame_allocator_allocate_is_span_free_1", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(0);
    REQUIRE(allocator.is_span_free(0, 0) == true);
}

TEST_CASE("test_page_frame_allocator_allocate_is_span_free_2", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(0);
    REQUIRE(allocator.is_span_free(0, 4096) == true);
}

TEST_CASE("test_page_frame_allocator_allocate_is_span_free_3", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(4096);
    REQUIRE(allocator.is_free(1*4096) == true);
    REQUIRE(allocator.is_free(2*4096) == false);
    REQUIRE(allocator.is_span_free(4096, 2*4096) == false);
}

TEST_CASE("test_page_frame_allocator_allocate_is_span_free_4", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    uintptr_t begin = PAGE_SIZE * (CAPACITY - 1);
    allocator.deallocate(begin);
    REQUIRE(allocator.is_span_free(begin, 100*4096) == false);
}

TEST_CASE("test_page_frame_allocator_allocate_allocate_span_0", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    REQUIRE(allocator.allocate_span(0) == 0);
}

TEST_CASE("test_page_frame_allocator_allocate_allocate_span_1", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(4096);
    REQUIRE(allocator.allocate_span(0) == 4096);
}

TEST_CASE("test_page_frame_allocator_allocate_allocate_span_2", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(4096);
    REQUIRE(allocator.allocate_span(4096) == 4096);
}

TEST_CASE("test_page_frame_allocator_allocate_allocate_span_3", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(4096);
    REQUIRE(allocator.allocate_span(2*4096) == 0);
}

TEST_CASE("test_page_frame_allocator_allocate_allocate_span_4", "[page_frame_allocator]")
{
    TestingAllocator allocator;
    allocator.deallocate(4096);
    allocator.deallocate(2*4096);
    REQUIRE(allocator.allocate_span(5000) == 4096);
}
