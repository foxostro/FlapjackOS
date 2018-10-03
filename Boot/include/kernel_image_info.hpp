// The kernel linker script defines a number symbols related to the kernel
// executable image. This header defines them for use in C++ code.
#ifndef FLAPJACKOS_BOOT_INCLUDE_KERNEL_IMAGE_INFO_HPP
#define FLAPJACKOS_BOOT_INCLUDE_KERNEL_IMAGE_INFO_HPP

#include "boot.h"
#include <cstddef>
#include <cstdint>

// The first address of the kernel image in physical memory.
extern "C" char g_kernel_physical_start[];

// The end of the boot section in memory.
extern "C" char g_boot_end[];

// The first address of the kernel image in memory.
extern "C" char g_kernel_image_begin[];

// The beginning of the text section in memory.
extern "C" char g_kernel_text_begin[];

// The end of the text section in memory.
extern "C" char g_kernel_text_end[];

// The beginning of the kernel rodata section in memory.
extern "C" char g_kernel_rodata_begin[];

// The end of the kernel rodata section in memory.
extern "C" char g_kernel_rodata_end[];

// The beginning of the C++ global constructors.
extern char g_start_ctors[];

// The end of the C++ global constructors.
extern char g_end_ctors[];

// The end of the kernel data section in memory.
extern "C" char g_kernel_data_end[];

// The end of the kernel image in memory.
extern "C" char g_kernel_image_end[];

// The end of the kernel image in physical memory.
extern "C" char g_kernel_physical_end[];

#endif // FLAPJACKOS_BOOT_INCLUDE_KERNEL_IMAGE_INFO_HPP
