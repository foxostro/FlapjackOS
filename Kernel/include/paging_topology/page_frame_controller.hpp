#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_FRAME_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_FRAME_CONTROLLER_HPP

#include <page_size.hpp>

namespace PagingTopology {

// Owns a page frame.
class PageFrameController {
public:
    virtual ~PageFrameController() = default;

    // Returns the physical address of the page frame.
    // Address zero is reserved to represent the null state.
    virtual uintptr_t get_page_frame() const = 0;
};

} // namespace PagingTopology

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGING_TOPOLOGY_PAGE_FRAME_CONTROLLER_HPP
