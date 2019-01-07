#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP

#include <platform/i386/generic_page_map_level_two_controller.hpp>
#include <platform/i386/page_map_level_one_controller.hpp>
#include <platform/i386/page_directory.hpp>

namespace i386 {

struct PageMapLevelTwoControllerPolicy {
    using MyPageTable = i386::PageTable;
    using MyPageDirectory = i386::PageDirectory;
    using ConcretePageMapLevelOneController = i386::PageMapLevelOneController;
    static constexpr size_t SIZE_GOVERENED_BY_ENTRY = 0x400000;
};

class PageMapLevelTwoController final : public GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy> {
public:
    PageMapLevelTwoController(HardwareMemoryManagementUnit& mmu)
     : GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy>(mmu)
    {}

    PageMapLevelTwoController(HardwareMemoryManagementUnit& mmu,
                              UniquePointer<MyPageDirectory> pd)
     : GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy>(mmu, std::move(pd))
    {}

    size_t get_index_of_pml2_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr uint32_t PDE_INDEX_SHIFT = 22;
        size_t index = linear_address >> PDE_INDEX_SHIFT;
        return index;
    }

    size_t get_index_of_pml1_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr uint32_t PTE_INDEX_SHIFT = 12;
        constexpr uint32_t PTE_INDEX_MASK = 0x03FF;
        size_t index = linear_address >> PTE_INDEX_SHIFT & PTE_INDEX_MASK;
        return index;
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
