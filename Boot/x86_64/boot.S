#include "multiboot_asm.h"
#include "paging.h"
#include "creg_bits.h"
#include "declare_gdt.h"
#include "panic_asm.h"
#include "seg.h"


DECLARE_MULTIBOOT_HEADER


.section .boottext


.code32


.global _start
.type _start, @function
_start:
    cli
    cld

    # Check whether the CPU supports long mode, panic if it does not.
    # I couldn't find documentation in the Intel manuals describing this, but
    # it is described here: <https://www.felixcloutier.com/x86/cpuid>
    # Also, we need a stack at this point to save/restore the registers around
    # the call to cpuid.
    leal cpuid_stack, %esp
    pusha
    movl $0x80000001, %eax
    cpuid
    andl $(1<<29), %edx
    jnz has_x86_64_support
    PANIC missing_x86_64_support L1 L2
has_x86_64_support:
    popa

    # The kernel must lie entirely within the first 2MB of physical memory.
    # This is because a single page table can only map 2MB of memory, and the
    # procedure below only maps a single page table.
    # (The limit is 4MB for i386.)
    leal g_kernel_physical_end, %ecx
    cmpl $0x200000, %ecx
    jle kernel_is_not_too_big
    PANIC kernel_too_big_string L3 L4
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
    ljmp $SEGSEL_KERNEL_CS, $_start64

.size _start, . - _start


.code64


.global _start64
.type _start64, @function
_start64:
    # Setup segment selectors
    movw $SEGSEL_KERNEL_DS, %ax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Need to initialize SSE immediately on x86_64 because the C++ kernel will
    # expect to be able to use those instructions.
    # These features are guaranteed to be supported on x86_64 so we don't bother
    # to check the cpuid feature flags.
    movq %cr0, %rcx
    andq $(~(CR0_EM | CR0_TS | CR0_NE)), %rcx
    orq $(CR0_MP), %rcx
    movq %rcx, %cr0
    fninit

    movq %cr4, %rcx
    orq $(CR4_OSFXSR | CR4_OSXMMEXCPT), %rcx
    movq %rcx, %cr4

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

.align 4
.skip 0x20
cpuid_stack:

missing_x86_64_support:
.asciz "ERROR: The CPU does not support x86_64."

kernel_too_big_string:
.asciz "ERROR: The kernel is too big. Must be 2MB or less."


.section .bss

# In order to remap the kernel into higher memory, we need to reserve space
# for bootstrap paging structures. We use these to map physical memory into the
# kernel virtual memory region for the kernel image.
# The kernel must fit into 4MB of physical memory for this to work.
DECLARE_PML4 pml4
DECLARE_PAGE_DIRECTORY_POINTER_TABLE low_pdpt
DECLARE_PAGE_DIRECTORY_POINTER_TABLE high_pdpt
DECLARE_PAGE_DIRECTORY low_page_directory
DECLARE_PAGE_DIRECTORY high_page_directory
DECLARE_PAGE_TABLE low_page_table
DECLARE_PAGE_TABLE high_page_table
DECLARE_KERNEL_STACK
