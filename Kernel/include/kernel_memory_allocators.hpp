#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_MEMORY_ALLOCATORS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_MEMORY_ALLOCATORS_HPP

#include <cstdlib>
#include <multiboot.h>
#include <common/text_terminal.hpp>
#include <kernel_contiguous_memory_allocator.hpp>
#include <page_frame_allocator_factory.hpp>

// Initializes the kernel's core memory allocators.
// Instantiated at boot time.
class KernelMemoryAllocators {
public:
    // Create the singleton kernel memory allocator instance.
    static KernelMemoryAllocators* create(multiboot_info_t *mb_info,
                                          TextTerminal &terminal);

    KernelBreakAllocator& get_kernel_break_allocator()
    {
        return kernel_break_allocator_;
    }

    KernelContiguousMemoryAllocator& get_kernel_heap_allocator()
    {
        return *contiguous_memory_allocator_;
    }

    PageFrameAllocator& get_page_frame_allocator()
    {
        return *page_frame_allocator_;
    }

private:
    multiboot_info_t *mb_info_;
    TextTerminal &terminal_;
    KernelBreakAllocator kernel_break_allocator_;
    KernelContiguousMemoryAllocator *contiguous_memory_allocator_;
    PageFrameAllocator *page_frame_allocator_;

    KernelMemoryAllocators(multiboot_info_t *mb_info,
                           TextTerminal &terminal);

    void report_installed_memory();
    void report_free_page_frames();
    void initialize_kernel_break_allocator();
    void initialize_page_frame_allocator();
    void prepare_kernel_memory_map();
    void initialize_contiguous_memory_allocator();
    void initialize_kernel_malloc();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_MEMORY_ALLOCATORS_HPP
