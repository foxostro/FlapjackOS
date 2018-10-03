#ifndef FLAPJACKOS_BOOT_INCLUDE_PANIC_ASM_H
#define FLAPJACKOS_BOOT_INCLUDE_PANIC_ASM_H

#ifndef __ASSEMBLER__
#error "This header is intended for use only in assembly code."
#endif

.macro PANIC STRING
leal 0xB8000, %edi
leal \STRING, %esi
panic_print_loop:
xor %eax, %eax
movb (%esi), %al
cmpb $0, %al
je panic_halt_loop
movl %eax, (%edi)
incl %edi
movl $0b00000111, (%edi)
incl %edi
incl %esi
jmp panic_print_loop
panic_halt_loop:
cli
hlt
jmp panic_halt_loop
.endm

#endif // FLAPJACKOS_BOOT_INCLUDE_PANIC_ASM_H
