#ifndef FLAPJACKOS_BOOT_INCLUDE_MULTIBOOT_ASM_H
#define FLAPJACKOS_BOOT_INCLUDE_MULTIBOOT_ASM_H

#ifndef __ASSEMBLER__
#error "This header is intended for use only in assembly code."
#endif

// Declare the multiboot header.
.macro DECLARE_MULTIBOOT_HEADER
.set ALIGN,    1<<0
.set MEMINFO,  1<<1
.set FLAGS,    ALIGN | MEMINFO
.set MAGIC,    0x1BADB002
.set CHECKSUM, -(MAGIC + FLAGS)
.section .multiboot
.align 4
.long MAGIC
.long FLAGS
.long CHECKSUM
.endm

#endif // FLAPJACKOS_BOOT_INCLUDE_MULTIBOOT_ASM_H
