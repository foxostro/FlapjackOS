#include <seg.h>

.global lgdt
lgdt:
    movl 4(%esp), %eax
    movl 8(%esp), %ecx
    pushl %eax
    pushw %cx
    lgdt (%esp)
    ljmp $SEGSEL_KERNEL_CS, $lgdt_reload # Do a long jump to force a GDT reload.
lgdt_reload:
    movl $SEGSEL_KERNEL_DS, %eax # Load up the data segment selectors.
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    addl $6, %esp
    ret
