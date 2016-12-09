#include <seg.h>

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

.global init_tss # TODO: define in readable code
init_tss:
    .space 8
    .word 0x0018
    .space 92
    .word 0x0068

# Setup a 16KiB stack with 16 byte alignment.
.section .bss
.align 16
stack_bottom:
.skip 16384
stack_top:

.section .text

.global _start
.type _start, @function
_start:
    # Setup the initial kernel stack and jump to the kernel entry point.
    mov $stack_top, %esp
    mov $0, %ebp # back traces end here
    cld
    call kernel_main

    # Halt the machine forever. 
    cli
1:  hlt
    jmp 1b

.size _start, . - _start
