#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_CONFIGURATION_OPERATION_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_CONFIGURATION_OPERATION_HPP

#include <multiboot.h>
#include <logical_addressing.hpp>
#include <page_frame_allocator.hpp>

// Configures a page frame allocator for use at boot time.
//
// Enumerate the useable page frames of the system. Those which map to a VMA
// beyond the kernel break pointer are considered to be available for use.
// All other page frames are assumed to be in use, e.g., by the kernel image.
//
// "PageFrameEnumerator" is a Functor which enumerates the page frames in the
// system. This generally is implemented with logic to walk the multiboot memory
// map.
template<typename PageFrameEnumerator>
class PageFrameAllocatorConfigurationOperation {
public:
    PageFrameAllocatorConfigurationOperation(uintptr_t break_address,
                                             PageFrameEnumerator enumerator)
     : break_address_(break_address),
       enumerator_(enumerator)
    {}

    void configure(PageFrameAllocator &page_frames)
    {
        page_frames.mark_all_as_used();
        enumerate_page_frames([&](uintptr_t page_frame){
            uintptr_t corresponding_logical_address = convert_physical_to_logical_address(page_frame);
            if (corresponding_logical_address >= break_address_) {
                page_frames.deallocate(page_frame);
            }
        });
    }

private:
    const uintptr_t break_address_;
    PageFrameEnumerator enumerator_;

    // Use the provided enumerator to enumerate page frames in the system.
    // For each page frame, invoke the function `fn'.
    template<typename Function>
    void enumerate_page_frames(Function &&fn)
    {
        while (enumerator_.has_next()) {
            uintptr_t page_frame = enumerator_.get_next();
            fn(page_frame);
        }
    }
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_CONFIGURATION_OPERATION_HPP
