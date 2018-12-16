#include "catch.hpp"
#include "page_frame_allocator_configuration_operation.hpp"
#include "boot.h"

// AFOX_TODO: Need a better way to share test files across libraries.
#include "../../KernelPlatformSupport/test/mock_memory_management_unit.hpp"

class DummyPageFrameEnumerator {
public:
    DummyPageFrameEnumerator(size_t count)
    {
        for (size_t i = 0; i < count; ++i) {
            uintptr_t page_frame = PAGE_SIZE * i + KERNEL_PHYSICAL_LOAD_ADDR;
            page_frames_.push_back(page_frame);
        }
        index_ = 0;
    }

    DummyPageFrameEnumerator(const DummyPageFrameEnumerator &enumerator) = default;

    bool has_next() const
    {
        return index_ < page_frames_.size();
    }

    uintptr_t get_next()
    {
        assert(has_next());
        uintptr_t result = page_frames_[index_];
        index_++;
        return result;
    }

private:
    std::vector<uintptr_t> page_frames_;
    size_t index_;
};

using Operation = PageFrameAllocatorConfigurationOperation<DummyPageFrameEnumerator, MockMemoryManagementUnit64>;

TEST_CASE("test_page_frame_allocator_configuration_ctor", "[page_frame_allocator_configuration_operation]")
{
    MockMemoryManagementUnit64 mmu;
    DummyPageFrameEnumerator enumerator(0);
    Operation op(0, enumerator, mmu);
}

TEST_CASE("test_page_frame_allocator_configuration_configure_all_free", "[page_frame_allocator_configuration_operation]")
{
    constexpr size_t NUMBER_OF_PAGE_FRAMES = 1;
    MockMemoryManagementUnit64 mmu;
    DummyPageFrameEnumerator enumerator(NUMBER_OF_PAGE_FRAMES);
    Operation op(0, enumerator, mmu);
    PageFrameAllocator page_frame_allocator;

    op.configure(page_frame_allocator);

    REQUIRE(page_frame_allocator.get_number_of_free_page_frames() == NUMBER_OF_PAGE_FRAMES);
}

TEST_CASE("test_page_frame_allocator_configuration_configure_none_free", "[page_frame_allocator_configuration_operation]")
{
    constexpr size_t NUMBER_OF_PAGE_FRAMES = 1;
    MockMemoryManagementUnit64 mmu;
    DummyPageFrameEnumerator enumerator(NUMBER_OF_PAGE_FRAMES);
    Operation op(~0, enumerator, mmu);
    PageFrameAllocator page_frame_allocator;
    
    op.configure(page_frame_allocator);

    REQUIRE(page_frame_allocator.get_number_of_free_page_frames() == 0);
}
