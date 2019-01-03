#include "catch.hpp"
#include <platform/i386/page_frame_controller.hpp>
#include <platform/i386/page_map_level_one_controller.hpp>
#include <page_frame_allocator.hpp>

constexpr size_t MAX_PAGE_FRAMES = 1024;
using TestingAllocator = PageFrameAllocator_<MAX_PAGE_FRAMES>;
using PML1 = i386::PageMapLevelOneController<TestingAllocator>;

static TestingAllocator create_page_frame_allocator()
{
    TestingAllocator allocator;
    for (size_t i = 1; i < MAX_PAGE_FRAMES; ++i) {
        allocator.deallocate(PAGE_SIZE * i);
    }
    return allocator;
}

TEST_CASE("i386::PageMapLevelOneController -- PML1 has 1024 entries", "[i386]")
{
    PML1 page_table;
    REQUIRE(1024 == page_table.get_number_of_entries());
}

TEST_CASE("i386::PageMapLevelOneController -- page table is initially empty", "[i386]")
{
    PML1 page_table;
    for (size_t i = 0, n = page_table.get_number_of_entries(); i < n; ++i) {
        auto entry = page_table.get_entry(i);
        REQUIRE(entry->get_page_frame() == nullptr);
        REQUIRE(entry->is_present() == false);
    }
}

TEST_CASE("i386::PageMapLevelOneController -- ensure we can stick page frames in the table", "[i386]")
{
    TestingAllocator allocator = create_page_frame_allocator();
    PML1 page_table;

    {
        auto entry = page_table.get_entry(0);
        entry->set_page_frame(new i386::PageFrameController{allocator});
    }

    {
        auto entry = page_table.get_entry(0);
        REQUIRE(entry->get_page_frame() != nullptr);
        REQUIRE(entry->is_present() == true);
    }
}

TEST_CASE("i386::PageMapLevelOneController -- ensure we can remove page frame from the table", "[i386]")
{
    TestingAllocator allocator = create_page_frame_allocator();
    PML1 page_table;
    uintptr_t addr = 0;

    {
        auto entry = page_table.get_entry(0);
        entry->set_page_frame(new i386::PageFrameController{allocator});
        addr = entry->get_page_frame()->get_page_frame();
    }

    {
        auto entry = page_table.get_entry(0);
        entry->set_page_frame(nullptr);
    }

    {
        auto entry = page_table.get_entry(0);
        REQUIRE(entry->get_page_frame() == nullptr);
        REQUIRE(entry->is_present() == false);
        REQUIRE(allocator.is_free(addr) == true);
    }
}

TEST_CASE("i386::PageMapLevelOneController -- ctor can accept a raw poinnter to a page table", "[i386]")
{
    TestingAllocator allocator = create_page_frame_allocator();
    i386::PageTable* page_table = new i386::PageTable;
    PML1 controller{page_table};
    auto entry = controller.get_entry(0);
    entry->set_page_frame(new i386::PageFrameController{allocator});
    uintptr_t addr = entry->get_page_frame()->get_page_frame();
    REQUIRE(addr == page_table->entries[0].get_address());
}