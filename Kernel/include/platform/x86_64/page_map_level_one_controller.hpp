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

using PageMapLevelOneController = i386::GenericPageMapLevelOneController<PageMapLevelOneControllerPolicy>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_ONE_CONTROLLER_HPP
