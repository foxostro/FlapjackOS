#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_CONFIGURATION_OPERATION_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_CONFIGURATION_OPERATION_HPP

#include <multiboot.h>
#include <page_frame_allocator.hpp>
#include <page_size.hpp>
#include <hardware_memory_management_unit.hpp>
#include <boot.h> // for KERNEL_VIRTUAL_START_ADDR
#include <logical_addressing.hpp> // for KERNEL_MEMORY_REGION

// Configures a page frame allocator for use at boot time.
//
// Enumerate the useable page frames of the system. Those which map to a VMA
// beyond the kernel break pointer are considered to be available for use.
// All other page frames are assumed to be in use, e.g., by the kernel image.
//
// "PageFrameEnumerator" is a Functor which enumerates the page frames in the
// system. This generally is implemented with logic to walk the multiboot memory
// map.
template<typename PageFrameEnumerator, typename ModuleEnumerator>
class PageFrameAllocatorConfigurationOperation {
public:
    PageFrameAllocatorConfigurationOperation(uintptr_t break_address,
                                             PageFrameEnumerator page_frame_enumerator,
                                             ModuleEnumerator module_enumerator,
                                             HardwareMemoryManagementUnit& mmu)
     : break_address_(break_address),
       page_frame_enumerator_(page_frame_enumerator),
       module_enumerator_(module_enumerator),
       mmu_(mmu)
    {}

    void configure(PageFrameAllocator& page_frames)
    {
        page_frames.mark_all_as_used();
        mark_unavailable_memory(page_frames);
        mark_modules_memory(page_frames);
    }

private:
    const uintptr_t break_address_;
    PageFrameEnumerator page_frame_enumerator_;
    ModuleEnumerator module_enumerator_;
    HardwareMemoryManagementUnit& mmu_;

    // Use the provided enumerator to enumerate page frames in the system.
    // For each page frame, invoke the function `fn'.
    template<typename Function>
    void enumerate_page_frames(Function&& fn)
    {
        while (page_frame_enumerator_.has_next()) {
            uintptr_t page_frame = page_frame_enumerator_.get_next();
            fn(page_frame);
        }
    }

    void mark_unavailable_memory(PageFrameAllocator& page_frames)
    {
        enumerate_page_frames([&](uintptr_t page_frame){
            if (is_page_frame_in_available_region(page_frame)) {
                page_frames.deallocate(page_frame);
            }
        });
    }

    bool is_page_frame_in_available_region(uintptr_t page_frame)
    {
        uintptr_t logical_address = mmu_.convert_physical_to_logical_address(page_frame);
        return logical_address >= break_address_ && logical_address < KERNEL_VIRTUAL_START_ADDR+KERNEL_MEMORY_REGION_SIZE;
    }

    void mark_modules_memory(PageFrameAllocator& page_frames)
    {
        module_enumerator_.enumerate([&](multiboot_module_t& module){
            for (uintptr_t this_page_frame = module.mod_start;
                 this_page_frame < module.mod_end;
                 this_page_frame += PAGE_SIZE) {

                page_frames.allocate_specific_page_frame(this_page_frame);
            }
        });
    }
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_CONFIGURATION_OPERATION_HPP
