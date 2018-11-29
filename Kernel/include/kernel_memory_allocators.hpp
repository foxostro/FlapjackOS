#ifndef FLAPJACKOS_KERNEL_INCLUDE_KERNEL_MEMORY_ALLOCATORS_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_KERNEL_MEMORY_ALLOCATORS_HPP

#include <cstdlib>
#include <multiboot.h>
#include <common/text_terminal.hpp>
#include <page_frame_allocator.hpp>

// Initializes the kernel's core memory allocators.
// Instantiated at boot time.
class KernelMemoryAllocators {
public:
    // Create the singleton kernel memory allocator instance.
    static KernelMemoryAllocators* create(multiboot_info_t *mb_info,
                                          TextTerminal &terminal);

    PageFrameAllocator& get_page_frame_allocator()
    {
        return page_frame_allocator_;
    }

private:
    multiboot_info_t *mb_info_;
    TextTerminal &terminal_;
    PageFrameAllocator page_frame_allocator_;

    static constexpr size_t HEAP_SIZE = 4096;
    using HeapStorage = typename std::aligned_storage<HEAP_SIZE, alignof(int)>::type;
    HeapStorage heap_storage_;

    KernelMemoryAllocators(multiboot_info_t *mb_info,
                           TextTerminal &terminal);

    void report_installed_memory();
    void report_free_page_frames();
    void initialize_page_frame_allocator();
    void initialize_kernel_malloc();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_KERNEL_MEMORY_ALLOCATORS_HPP
