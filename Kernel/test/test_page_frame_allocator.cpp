#include "catch.hpp"
#include "page_frame_allocator.hpp"

constexpr size_t CAPACITY = 10;
using TestingAllocator = PageFrameAllocator_<CAPACITY>;

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
