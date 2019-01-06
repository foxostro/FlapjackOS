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

using PageMapLevelOneController = GenericPageMapLevelOneController<PageMapLevelOneControllerPolicy>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
