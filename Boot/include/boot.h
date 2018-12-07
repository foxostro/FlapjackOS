// Defines used in the boot assembly start procedure. This should match the
// values found in "kernel_image_info.hpp" in libKernel.
#ifndef FLAPJACKOS_BOOT_INCLUDE_BOOT_H
#define FLAPJACKOS_BOOT_INCLUDE_BOOT_H

// The virtual address of the higher half.
// This is the beginning of a virtual memory region which maps to contiguous,
// low, physical memory and contains the kernel image.
// This is also the first kernel logical address.
#define KERNEL_VIRTUAL_START_ADDR_64BIT 0xFFFFFFFFC0000000
#define KERNEL_VIRTUAL_START_ADDR_32BIT 0xC0000000
#ifdef __c_plus_plus
#ifdef __x86_64__
constexpr uintptr_t KERNEL_VIRTUAL_START_ADDR = KERNEL_VIRTUAL_START_ADDR_64BIT;
#else
constexpr uintptr_t KERNEL_VIRTUAL_START_ADDR = KERNEL_VIRTUAL_START_ADDR_32BIT;
#endif
#else // defined(__c_plus_plus)
#ifdef __x86_64__
#define KERNEL_VIRTUAL_START_ADDR KERNEL_VIRTUAL_START_ADDR_64BIT
#else
#define KERNEL_VIRTUAL_START_ADDR KERNEL_VIRTUAL_START_ADDR_32BIT
#endif
#endif // defined(__c_plus_plus)

// The physical address where the kernel image is loaded into memory.
#define KERNEL_PHYSICAL_LOAD_ADDR 0x100000

#endif // FLAPJACKOS_BOOT_INCLUDE_BOOT_H
