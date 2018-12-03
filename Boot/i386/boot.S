#include "multiboot_asm.h"
#include "paging.h"
#include "creg_bits.h"
#include "panic_asm.h"


DECLARE_MULTIBOOT_HEADER


.section .boottext
.global _start
.type _start, @function
_start:
    cli
    cld

    # The kernel must lie entirely within the first 4MB of physical memory.
    # This is because a single page table can only map 4MB of memory, and the
    # procedure below only maps a single page table.
    # (The limit is 2MB for x86_64.)
    leal g_kernel_physical_end, %ecx
    cmpl $0x400000, %ecx
    jle kernel_is_not_too_big
    PANIC kernel_too_big_string
kernel_is_not_too_big:

    # This will map the entire kernel image as read-write, but we'll fix this up
    # later to mark kernel rodata and text sections read-only.
    FILL_PAGE_TABLE GET_PHYSICAL_ADDRESS(bootstrap_page_table), 0x000000

    # Fill out the bootstrap page directory.
    # We use the first page table twice in the page directory in order to effect
    # 1) a direct mapping of the first 4MB of physical memory to the first 4MB
    # of virtual memory as well as 2) a mapping of physical memory to the kernel
    # virtual start address at KERNEL_VIRTUAL_START_ADDR.
    # It's necessary to map the table twice to avoid page faulting when we try
    # to fetch the next instruction before entering the higher half.
    SET_PAGE_DIRECTORY_ENTRY GET_PHYSICAL_ADDRESS(bootstrap_page_directory), GET_PHYSICAL_ADDRESS(bootstrap_page_table), 0x000000
    SET_PAGE_DIRECTORY_ENTRY GET_PHYSICAL_ADDRESS(bootstrap_page_directory), GET_PHYSICAL_ADDRESS(bootstrap_page_table), KERNEL_VIRTUAL_START_ADDR+0x000000

    # Point CR3 to the bootstrap page directory.
    movl $(GET_PHYSICAL_ADDRESS(bootstrap_page_directory)), %ecx
    movl %ecx, %cr3

    # Enable paging.
    movl %cr0, %ecx
    orl $(CR0_PG | CR0_PE | CR0_WP), %ecx
    movl %ecx, %cr0

    # Jump to higher half with an absolute jump.
    leal start_in_higher_half, %ecx
    jmpl *%ecx

.size _start, . - _start


.section .text

.global start_in_higher_half
.type start_in_higher_half, @function
start_in_higher_half:
    # Earlier, we mapped the page table twice to effect identity mapping of the
    # first 4MB of physical memory. Remove this mapping now that we're in
    # the higher half.
    movl $0, bootstrap_page_directory + 0

    # Force a TLB flush.
    movl %cr3, %ecx
    movl %ecx, %cr3

    # Setup a stack we can use in a C environment.
    leal stack_top, %esp
    movl $0, %ebp # back traces end here

    # Call the C entry point.
    # Second parameter is the address of the top of the stack.
    # First parameter is the address of the multiboot structure.
    # We need to convert this from a physical address to a linear address by
    # adding an offset to shift to the higher half.
    pushl %esp # Second parameter
    addl $(KERNEL_VIRTUAL_START_ADDR), %ebx
    pushl %ebx # First parameter
    call kernel_main

    # Halt the machine forever.
    # This is unreachable and is only here just-in-case.
    cli
hang:
    hlt
    jmp hang

.size start_in_higher_half, . - start_in_higher_half


.section .bootdata
kernel_too_big_string:
.asciz "The kernel is too big. Must be 4MB or less."


.section .bss
DECLARE_KERNEL_STACK

# In order to remap the kernel into higher memory, we need to reserve space
# for a bootstrap page directory and page table. We use these to map physical
# memory into the kernel virtual memory region for the kernel image.
# The kernel must fit into 4MB of physical memory for this to work.
DECLARE_PAGE_DIRECTORY bootstrap_page_directory
DECLARE_PAGE_TABLE bootstrap_page_table
