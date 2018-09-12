#include <seg.h>

.global lgdt
lgdt:
    # Move parameters into registers %eax and %ecx
    movl 4(%esp), %eax # Four byte base address
    movl 8(%esp), %ecx # Table limit. We only use two bytes of this.

    # Build a six byte descriptor on the stack using the parameters, now in the
    # %eax and %cx registers. The lgdt instruction wants the address of a
    # descriptor with an six byte base and a two byte limit.
    pushl %eax
    pushw %cx

    # Load the global descriptor table register (GDTR)
    lgdt (%esp)

    # Do a long jump to force a GDT reload.
    ljmp $SEGSEL_KERNEL_CS, $lgdt_reload
lgdt_reload:

    # Load up the data segment selectors.
    movl $SEGSEL_KERNEL_DS, %eax
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss

    # Cleanup the stack and return
    addl $6, %esp
    ret
