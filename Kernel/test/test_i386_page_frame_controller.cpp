#include "catch.hpp"
#include <platform/i386/page_frame_controller.hpp>
#include <page_frame_allocator.hpp>

constexpr size_t MAX_PAGE_FRAMES = 10;
using TestingAllocator = PageFrameAllocator_<MAX_PAGE_FRAMES>;
using TestingPageFrameController = i386::PageFrameController<TestingAllocator>;

static TestingAllocator create_page_frame_allocator()
{
    TestingAllocator allocator;
    for (size_t i = 1; i < MAX_PAGE_FRAMES; ++i) {
        allocator.deallocate(PAGE_SIZE * i);
    }
    return allocator;
}

TEST_CASE("i386::PageFrameController -- ctor allocates a page frame", "[i386]")
{
    auto allocator = create_page_frame_allocator();
    TestingPageFrameController page_frame{allocator};
    REQUIRE(allocator.is_free(page_frame.get_page_frame()) == false);
}

TEST_CASE("i386::PageFrameController -- dtor deallocates the page frame", "[i386]")
{
    auto allocator = create_page_frame_allocator();
    uintptr_t addr;

    {
        TestingPageFrameController page_frame{allocator};
        addr = page_frame.get_page_frame();
    }

    REQUIRE(allocator.is_free(addr) == true);
}

TEST_CASE("i386::PageFrameController -- move to reassign ownership of the page frame", "[i386]")
{
    auto allocator = create_page_frame_allocator();
    
    TestingPageFrameController page_frame1{allocator};
    uintptr_t addr1 = page_frame1.get_page_frame();

    TestingPageFrameController page_frame2{allocator};
    uintptr_t addr2 = page_frame2.get_page_frame();

    page_frame1 = std::move(page_frame2);

    REQUIRE(allocator.is_free(addr1) == true);
    REQUIRE(allocator.is_free(addr2) == false);
}

TEST_CASE("i386::PageFrameController -- move ctor reassigns ownership of the page frame", "[i386]")
{
    auto allocator = create_page_frame_allocator();
    
    TestingPageFrameController page_frame1{allocator};
    uintptr_t addr = page_frame1.get_page_frame();

    {
        TestingPageFrameController page_frame2{std::move(page_frame1)};
        REQUIRE(addr == page_frame2.get_page_frame());
    }

    REQUIRE(allocator.is_free(addr) == true);
}
