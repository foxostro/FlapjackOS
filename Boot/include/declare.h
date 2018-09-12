#ifndef FLAPJACKOS_BOOT_INCLUDE_DECLARE_H
#define FLAPJACKOS_BOOT_INCLUDE_DECLARE_H

#ifndef __ASSEMBLER__
#error "This header is intended for use only in assembly code."
#endif

// Define a global symbol in the current section.
.macro GLOBAL_SYMBOL NAME, SIZE, ALIGNMENT
.align \ALIGNMENT
.global \NAME
\NAME:
.skip \SIZE
.size \NAME , . - \NAME
.endm

// Define a non-global symbol in the current section.
.macro SYMBOL NAME, SIZE, ALIGNMENT
.align \ALIGNMENT
\NAME:
.skip \SIZE
.size \NAME , . - \NAME
.endm

// Define a symbol in the current section for the kernel stack.
.macro DECLARE_KERNEL_STACK
.set STACK_SIZE, 0x4000
.set STACK_ALIGNMENT, 16
SYMBOL stack_bottom, STACK_SIZE, STACK_ALIGNMENT
stack_top:
.endm

#endif // FLAPJACKOS_BOOT_INCLUDE_DECLARE_H
