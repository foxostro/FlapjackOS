#include "catch.hpp"
#include <platform/i386/page_map_level_one_controller.hpp>
#include <concrete_page_frame_allocator.hpp>
#include "mock_memory_management_unit.hpp"

constexpr size_t MAX_PAGE_FRAMES = 1024;
using TestingAllocator = GenericConcretePageFrameAllocator<MAX_PAGE_FRAMES>;
using PML1 = i386::PageMapLevelOneController;

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
    MockMemoryManagementUnit mmu;
    PML1 page_table{mmu};
    REQUIRE(1024 == page_table.get_number_of_entries());
}

TEST_CASE("i386::PageMapLevelOneController -- page table is initially empty", "[i386]")
{
    MockMemoryManagementUnit mmu;
    PML1 page_table{mmu};
    for (size_t i = 0, n = page_table.get_number_of_entries(); i < n; ++i) {
        auto& entry = page_table.get_entry(i);
        REQUIRE(entry.get_page_frame() == nullptr);
        REQUIRE(entry.is_present() == false);
    }
}

TEST_CASE("i386::PageMapLevelOneController -- ensure we can stick page frames in the table", "[i386]")
{
    TestingAllocator allocator = create_page_frame_allocator();
    MockMemoryManagementUnit mmu;
    PML1 page_table{mmu};

    {
        auto& entry = page_table.get_entry(0);
        entry.set_page_frame(new i386::PageFrameController{allocator});
    }

    {
        auto& entry = page_table.get_entry(0);
        REQUIRE(entry.get_page_frame() != nullptr);
        REQUIRE(entry.is_present() == true);
    }
}

TEST_CASE("i386::PageMapLevelOneController -- ensure we can remove page frame from the table", "[i386]")
{
    TestingAllocator allocator = create_page_frame_allocator();
    MockMemoryManagementUnit mmu;
    PML1 page_table{mmu};
    uintptr_t addr = 0;

    {
        auto& entry = page_table.get_entry(0);
        entry.set_page_frame(new i386::PageFrameController{allocator});
        addr = entry.get_page_frame()->get_page_frame();
    }

    {
        auto& entry = page_table.get_entry(0);
        entry.set_page_frame(nullptr);
    }

    {
        auto& entry = page_table.get_entry(0);
        REQUIRE(entry.get_page_frame() == nullptr);
        REQUIRE(entry.is_present() == false);
        REQUIRE(allocator.is_free(addr) == true);
    }
}

TEST_CASE("i386::PageMapLevelOneController -- ctor can accept a raw pointer to a page table", "[i386]")
{
    TestingAllocator allocator = create_page_frame_allocator();
    MockMemoryManagementUnit mmu;
    i386::PageTable* page_table = new i386::PageTable;
    PML1 controller{mmu, page_table};
    auto& entry = controller.get_entry(0);
    entry.set_page_frame(new i386::PageFrameController{allocator});
    uintptr_t addr = entry.get_page_frame()->get_page_frame();
    REQUIRE(addr == page_table->entries[0].get_address());
}

TEST_CASE("i386::PageMapLevelOneController -- ctor will not modify a page table that it receives", "[i386]")
{
    i386::PageTable* actual = new i386::PageTable;
    memset(actual, 0xAB, sizeof(i386::PageTable));
    i386::PageTable* expected = new i386::PageTable;
    memcpy(expected, actual, sizeof(i386::PageTable));
    MockMemoryManagementUnit mmu;
    PML1 controller{mmu, actual};
    REQUIRE(0 == memcmp(expected, actual, sizeof(i386::PageTable)));
}