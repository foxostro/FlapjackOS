#include <seg.h>

.global x86_64_lgdt
x86_64_lgdt:
#ifdef __x86_64__
    # Build a ten byte descriptor on the stack using the parameters passed in
    # the %rdi and %rsi registers. The lgdt instruction wants the address of a
    # descriptor with an eight byte base and a two byte limit.
    pushq %rdi
    pushw %si
    
    # Load the global descriptor table register (GDTR)
    lgdt (%rsp)

    movl $SEGSEL_KERNEL_DS, %eax # Load up the data segment selectors.
    movw %ax, %ds
    movw %ax, %es
    movw %ax, %fs
    movw %ax, %gs
    movw %ax, %ss
    
    # Cleanup the stack and return
    addq $10, %rsp
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_lgdt."
#endif