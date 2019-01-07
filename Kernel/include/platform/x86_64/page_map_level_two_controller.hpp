#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP

#include <platform/i386/generic_page_map_level_two_controller.hpp>
#include <platform/x86_64/page_map_level_one_controller.hpp>
#include <platform/x86_64/page_directory.hpp>

namespace x86_64 {

struct PageMapLevelTwoControllerPolicy {
    using MyPageTable = x86_64::PageTable;
    using MyPageDirectory = x86_64::PageDirectory;
    using ConcretePageMapLevelOneController = x86_64::PageMapLevelOneController;
    static constexpr size_t SIZE_GOVERENED_BY_ENTRY = 0x200000;
};

class PageMapLevelTwoController final : public i386::GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy> {
public:
    PageMapLevelTwoController(HardwareMemoryManagementUnit& mmu)
     : i386::GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy>(mmu)
    {}

    PageMapLevelTwoController(HardwareMemoryManagementUnit& mmu,
                              UniquePointer<MyPageDirectory> pd)
     : i386::GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy>(mmu, std::move(pd))
    {}

    size_t get_index_of_pml2_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr unsigned PDE_INDEX_SHIFT = 21;
        constexpr unsigned PDE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PDE_INDEX_SHIFT) & PDE_INDEX_MASK;
        return index;
    }

    size_t get_index_of_pml1_entry_by_address(uintptr_t linear_address) const override
    {
        constexpr unsigned PTE_INDEX_SHIFT = 12;
        constexpr unsigned PTE_INDEX_MASK = 0b111111111; // 9 bits
        size_t index = (linear_address >> PTE_INDEX_SHIFT) & PTE_INDEX_MASK;
        return index;
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
