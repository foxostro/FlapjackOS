#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP

#include <platform/i386/generic_page_map_level_one_controller.hpp>
#include <platform/i386/page_frame_controller.hpp>
#include <platform/x86_64/page_table.hpp>

namespace x86_64 {

struct PageMapLevelOneControllerPolicy {
    using MyPageTable = x86_64::PageTable;
    using MyPageFrameController = i386::PageFrameController;
};

class PageMapLevelOneController final : public i386::GenericPageMapLevelOneController<PageMapLevelOneControllerPolicy>
{
public:
    PageMapLevelOneController(HardwareMemoryManagementUnit& mmu)
     : i386::GenericPageMapLevelOneController<PageMapLevelOneControllerPolicy>(mmu)
    {}

    PageMapLevelOneController(HardwareMemoryManagementUnit& mmu,
                              UniquePointer<MyPageTable> pt)
     : i386::GenericPageMapLevelOneController<PageMapLevelOneControllerPolicy>(mmu, std::move(pt))
    {}

    size_t get_index_of_pml1_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr unsigned PTE_INDEX_SHIFT = 12;
        constexpr unsigned PTE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PTE_INDEX_SHIFT) & PTE_INDEX_MASK;
        return index;
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
