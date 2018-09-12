#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_FACTORY_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_FACTORY_HPP

#include <multiboot.h>
#include <logical_addressing.hpp>
#include <panic.h>
#include <page_frame_allocator.hpp>
#include <kernel_break_allocator.hpp>
#include <common/text_terminal.hpp>
#include <page_size.hpp>

// Creates the singleton page frame allocator.
class PageFrameAllocatorFactory {
public:
    PageFrameAllocatorFactory(multiboot_info_t *mb_info,
                              KernelBreakAllocator &break_allocator,
                              TextTerminal &terminal);

    PageFrameAllocator* create();

    template<typename Function>
    void enumerate_page_frames(Function &&fn)
    {
        if (!(mb_info_->flags & MULTIBOOT_MEMORY_INFO)) {
            panic("The bootloader did not provide memory information.");
        }

        multiboot_memory_map_t *entry = (multiboot_memory_map_t *)convert_physical_to_logical_address(mb_info_->mmap_addr);
        multiboot_memory_map_t *limit = entry + mb_info_->mmap_length;

        while (entry < limit && entry->size>0) {
            if (entry->type == MULTIBOOT_MEMORY_AVAILABLE) {
                uintptr_t page_frame = entry->base_addr_low;
                for (size_t length = entry->length_low; length>0; length-=PAGE_SIZE, page_frame+=PAGE_SIZE) {
                    fn(page_frame);
                }
            }

            entry = (multiboot_memory_map_t*)((uintptr_t)entry + entry->size + sizeof(entry->size));
        }
    }

private:
    multiboot_info_t *mb_info_;
    KernelBreakAllocator &break_allocator_;
    TextTerminal &terminal_;

    size_t count_page_frames();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FRAME_ALLOCATOR_FACTORY_HPP
