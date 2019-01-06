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
};

using PageMapLevelTwoController = i386::GenericPageMapLevelTwoController<PageMapLevelTwoControllerPolicy>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_TWO_CONTROLLER_HPP
