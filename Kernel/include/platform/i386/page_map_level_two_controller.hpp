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

using PageMapLevelTwoController = GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
