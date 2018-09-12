# Define some constants for the multiboot header.
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)

# Declare the multiboot header.
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM

# The virtual memory address at which the kernel is linked. It is the
# responsibility of the assembly boot procedure to setup paging such that the
# the kernel is mapped into virtual memory starting at this address.
.set KERNEL_VIRTUAL_START_ADDR, 0xC0000000

.global _start
.type _start, @function
_start:
    cli
    cld

    # Fill out the first bootstrap page table.
    # This will map the entire kernel image as read-write, but we'll fix this up
    # later to mark kernel rodata and text sections read-only.
    # EDI holds the physical address of the page table.
    movl $(bootstrap_page_table_1 - KERNEL_VIRTUAL_START_ADDR), %edi
    # ESI holds the first physical address to map.
    movl $0, %esi
#include "fill_page_table.S.inc"

    # Fill out the second bootstrap page table.
    # EDI holds the physical address of the page table.
    movl $(bootstrap_page_table_2 - KERNEL_VIRTUAL_START_ADDR), %edi
    # ESI holds the first physical address to map.
    movl $0x400000, %esi
#include "fill_page_table.S.inc"

    # Fill out the third bootstrap page table.
    # EDI holds the physical address of the page table.
    movl $(bootstrap_page_table_3 - KERNEL_VIRTUAL_START_ADDR), %edi
    # ESI holds the first physical address to map.
    movl $0x800000, %esi
#include "fill_page_table.S.inc"

    # Fill out the bootstrap page directory.
    # We use the first page table twice in the page directory in order to effect
    # 1) a direct mapping of the first 4MB of physical memory to the first 4MB
    # of virtual memory as well as 2) a mapping of physical memory to the kernel
    # virtual start address at KERNEL_VIRTUAL_START_ADDR.
    # It's necessary to map the first table twice to avoid page faulting when we
    # try to fetch the next instruction before entering the higher half.
    movl $(bootstrap_page_table_1 - KERNEL_VIRTUAL_START_ADDR + 0x003), bootstrap_page_directory - KERNEL_VIRTUAL_START_ADDR + 0
    movl $(bootstrap_page_table_1 - KERNEL_VIRTUAL_START_ADDR + 0x003), bootstrap_page_directory - KERNEL_VIRTUAL_START_ADDR + ((KERNEL_VIRTUAL_START_ADDR+0x000000) >> 22) * 4
    movl $(bootstrap_page_table_2 - KERNEL_VIRTUAL_START_ADDR + 0x003), bootstrap_page_directory - KERNEL_VIRTUAL_START_ADDR + ((KERNEL_VIRTUAL_START_ADDR+0x400000) >> 22) * 4
    movl $(bootstrap_page_table_3 - KERNEL_VIRTUAL_START_ADDR + 0x003), bootstrap_page_directory - KERNEL_VIRTUAL_START_ADDR + ((KERNEL_VIRTUAL_START_ADDR+0x800000) >> 22) * 4

    # Point CR3 to the bootstrap page directory.
    movl $(bootstrap_page_directory - KERNEL_VIRTUAL_START_ADDR), %ecx
    movl %ecx, %cr3

    # Enable paging.
    movl %cr0, %ecx
    orl $0x80010000, %ecx
    movl %ecx, %cr0

    # Jump to higher half with an absolute jump. 
    leal start_in_higher_half, %ecx
    jmpl *%ecx

start_in_higher_half:
    # Above, we mapped the page table twice to effect identity mapping of the
    # first 4MB of physical memory. Remove this mapping now that we're in
    # the higher half.
    movl $0, bootstrap_page_directory + 0

    # Force a TLB flush.
    movl %cr3, %ecx
    movl %ecx, %cr3

    # Call the C entry point passing the multiboot structure as a parameter.
    leal stack_top, %esp
    movl $0, %ebp # back traces end here
    pushl %esp
    addl $(KERNEL_VIRTUAL_START_ADDR), %ebx
    pushl %ebx
    call kernel_main

    # Halt the machine forever.
    # This is unreachable and is only here just-in-case.
    cli
hang:
    hlt
    jmp hang

.size _start, . - _start

# In order to remap the kernel into higher memory, we need to reserve space
# for a bootstrap page directory and page tables. We use these to map physical
# memory into the kernel virtual memory region for the kernel image and for the
# bootstrap kernel heap.
.section .bss
.align 4096
.global bootstrap_page_directory
.size bootstrap_page_directory, 4096
bootstrap_page_directory:
.skip 4096
bootstrap_page_table_1:
.skip 4096
bootstrap_page_table_2:
.skip 4096
bootstrap_page_table_3:
.skip 4096

# Setup a 16KiB stack with 16 byte alignment.
.section .data
.align 16
stack_bottom:
.skip 16384
stack_top: