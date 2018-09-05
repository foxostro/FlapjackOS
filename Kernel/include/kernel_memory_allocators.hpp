#pragma once

#include <cstdlib>
#include <multiboot.h>
#include <common/text_terminal.hpp>
#include <kernel_heap_allocator.hpp>
#include <page_frame_allocator_factory.hpp>
#include <kernel_page_directory_populate_operation.hpp>
#include <cleanup_kernel_memory_map_operation.hpp>

// Initializes the kernel's core memory allocators.
// Instantiated at boot time.
class kernel_memory_allocators {
public:
    // Create the singleton kernel memory allocator instance.
    static kernel_memory_allocators*
    create(multiboot_info_t *mb_info,
           text_terminal &terminal);

    kernel_break_allocator& get_kernel_break_allocator()
    {
        return kernel_break_allocator_;
    }

    kernel_heap_allocator& get_kernel_heap_allocator()
    {
        return *kernel_heap_allocator_;
    }

    page_frame_allocator& get_page_frame_allocator()
    {
        return *page_frame_allocator_;
    }

private:
    multiboot_info_t *mb_info_;
    text_terminal &terminal_;
    kernel_break_allocator kernel_break_allocator_;
    kernel_heap_allocator *kernel_heap_allocator_;
    page_frame_allocator *page_frame_allocator_;

    kernel_memory_allocators(multiboot_info_t *mb_info,
                             text_terminal &terminal);

    void report_installed_memory();
    void report_free_page_frames();
    void initialize_kernel_break_allocator();
    void initialize_page_frame_allocator();
    void prepare_kernel_memory_map();
    void initialize_kernel_heap_allocator();
    void initialize_kernel_malloc();
};
