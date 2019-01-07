#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP

#include <platform/i386/generic_page_map_level_one_controller.hpp>
#include <platform/i386/page_table.hpp>
#include <platform/i386/page_frame_controller.hpp>

namespace i386 {

struct PageMapLevelOneControllerPolicy {
    using MyPageTable = i386::PageTable;
    using MyPageFrameController = i386::PageFrameController;
};

class PageMapLevelOneController final : public GenericPageMapLevelOneController<PageMapLevelOneControllerPolicy>
{
public:
    PageMapLevelOneController(HardwareMemoryManagementUnit& mmu)
     : GenericPageMapLevelOneController(mmu)
    {}

    PageMapLevelOneController(HardwareMemoryManagementUnit& mmu,
                              UniquePointer<MyPageTable> pt)
     : GenericPageMapLevelOneController(mmu, std::move(pt))
    {}

    size_t get_index_of_pml1_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr uint32_t PTE_INDEX_SHIFT = 12;
        constexpr uint32_t PTE_INDEX_MASK = 0x03FF;
        size_t index = linear_address >> PTE_INDEX_SHIFT & PTE_INDEX_MASK;
        return index;
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
