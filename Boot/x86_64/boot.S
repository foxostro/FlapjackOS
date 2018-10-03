#include "multiboot_asm.h"
#include "paging.h"
#include "creg_bits.h"
#include "declare_gdt.h"
#include "panic_asm.h"


DECLARE_MULTIBOOT_HEADER


.section .boottext


.code32


.global _start
.type _start, @function
_start:
    cli
    cld

    # The kernel must lie entirely within the first 4MB of physical memory.
    leal g_kernel_physical_end, %ecx
    cmpl $0x400000, %ecx
    jle kernel_is_not_too_big
    PANIC kernel_too_big_string
kernel_is_not_too_big:

    SET_PML4_ENTRY GET_PHYSICAL_ADDRESS(pml4), GET_PHYSICAL_ADDRESS(low_pdpt), KERNEL_PHYSICAL_LOAD_ADDR
    SET_PML4_ENTRY GET_PHYSICAL_ADDRESS(pml4), GET_PHYSICAL_ADDRESS(high_pdpt), KERNEL_PHYSICAL_LOAD_ADDR + KERNEL_VIRTUAL_START_ADDR

    SET_PDPT_ENTRY GET_PHYSICAL_ADDRESS(low_pdpt), GET_PHYSICAL_ADDRESS(low_page_directory), KERNEL_PHYSICAL_LOAD_ADDR
    SET_PDPT_ENTRY GET_PHYSICAL_ADDRESS(high_pdpt), GET_PHYSICAL_ADDRESS(high_page_directory), KERNEL_PHYSICAL_LOAD_ADDR + KERNEL_VIRTUAL_START_ADDR

    SET_PAGE_DIRECTORY_ENTRY GET_PHYSICAL_ADDRESS(low_page_directory), GET_PHYSICAL_ADDRESS(low_page_table), KERNEL_PHYSICAL_LOAD_ADDR
    SET_PAGE_DIRECTORY_ENTRY GET_PHYSICAL_ADDRESS(high_page_directory), GET_PHYSICAL_ADDRESS(high_page_table), KERNEL_PHYSICAL_LOAD_ADDR + KERNEL_VIRTUAL_START_ADDR

    FILL_PAGE_TABLE GET_PHYSICAL_ADDRESS(low_page_table), 0
    FILL_PAGE_TABLE GET_PHYSICAL_ADDRESS(high_page_table), 0

    # Pass the physical address of paging structures to the CPU.
    movl $(GET_PHYSICAL_ADDRESS(pml4)), %eax
    movl %eax, %cr3

    # Enable PAE as it is required for long mode.
    movl $(CR4_PAE), %eax
    movl %eax, %cr4

    # Enable long mode.
    movl $MSR_EFER, %ecx
    rdmsr
    or $MSR_EFER_LME, %eax
    wrmsr

    # Enable paging.
    movl %cr0, %ecx
    orl $(CR0_PG | CR0_WP), %ecx
    movl %ecx, %cr0

    # We need to have a GDT to run 64-bit code.
    lgdt gdt_ptr

    # Long jump into 64-bit code and never look back.
    ljmp $(KERNEL_GDT_ENTRY * GDT_ENTRY_SIZE), $_start64

.size _start, . - _start


.code64


.global _start64
.type _start64, @function
_start64:
    # Setup segment selectors
    movw $0, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Setup a stack we can use in a C environment.
    leaq stack_top, %rsp
    movq $0, %rbp # back traces end here

    # Call the C entry point.
    # Second parameter is the address of the top of the stack.
    # First parameter is the address of the multiboot structure.
    # We need to convert this from a physical address to a linear address by
    # adding an offset to shift to the higher half.
    movq %rsp, %rsi # Second parameter
    addq $(KERNEL_VIRTUAL_START_ADDR), %rbx
    movq %rbx, %rdi # First parameter
    callq kernel_main

    # Halt the machine forever.
    # This is unreachable and is only here just-in-case.
    cli
hang:
    hlt
    jmp hang

.size _start64, . - _start64


.section .bootdata
DECLARE_GDT gdt_table, gdt_table_end, gdt_ptr

kernel_too_big_string:
.asciz "The kernel is too big. Must be 4MB or less."


.section .bss

# In order to remap the kernel into higher memory, we need to reserve space
# for bootstrap paging structures. We use these to map physical memory into the
# kernel virtual memory region for the kernel image. The kernel must fit into
# 3MB of physical memory for this to work.
DECLARE_PML4 pml4
DECLARE_PAGE_DIRECTORY_POINTER_TABLE low_pdpt
DECLARE_PAGE_DIRECTORY_POINTER_TABLE high_pdpt
DECLARE_PAGE_DIRECTORY low_page_directory
DECLARE_PAGE_DIRECTORY high_page_directory
DECLARE_PAGE_TABLE low_page_table
DECLARE_PAGE_TABLE high_page_table
DECLARE_KERNEL_STACK