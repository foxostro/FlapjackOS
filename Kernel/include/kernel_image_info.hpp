#ifndef FLAPJACKOS_KERNEL_KERNEL_IMAGE_INFO_HPP
#define FLAPJACKOS_KERNEL_KERNEL_IMAGE_INFO_HPP

#include <cstddef>
#include <cstdint>
#include <page_size.hpp>

// The virtual address of the higher half.
// This is the beginning of a virtual memory region which maps to contiguous,
// low, physical memory and contains the kernel image.
// This is also the first kernel logical address.
constexpr uintptr_t KERNEL_VIRTUAL_START_ADDR = 0xC0000000;

// The bootstrap assembly start code will have configured some number of page
// tables for us to map the bootstrap heap into virtual memory.
constexpr size_t NUMBER_OF_BOOTSTRAP_PAGE_TABLES = 3;

// Compute the end of the bootstrap kernel heap. We can't go past this without
// mapping additional pages.
constexpr uintptr_t BOOTSTRAP_HEAP_END = KERNEL_VIRTUAL_START_ADDR + 0x400000 * NUMBER_OF_BOOTSTRAP_PAGE_TABLES;

// The first address of the kernel image in memory.
extern "C" char g_kernel_image_begin[];

// The end of the kernel image in memory.
extern "C" char g_kernel_image_end[];

// The end of the kernel rodata section in memory.
extern "C" char g_kernel_rodata_end[];

// The address of the bootstrap page directory.
// This is configured by the assembly bootstrap start code before jumping to the
// higher half.
extern "C" uint32_t g_bootstrap_page_directory[];

#endif // FLAPJACKOS_KERNEL_KERNEL_IMAGE_INFO_HPP
