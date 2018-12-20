# Loads the IDTR register.
.global i386_lidt
i386_lidt:
#ifdef __i386__
    # Move parameters into registers %eax and %ecx
    movl 4(%esp), %eax # Four byte base address
    movl 8(%esp), %ecx # Two byte table limit

    # Build a six byte descriptor on the stack using the parameters, now in the
    # %eax and %cx registers. The lidt instruction wants the address of a
    # descriptor with an six byte base and a two byte limit.
    pushl %eax
    pushw %cx

    # Load the interrupt descriptor table register (IDTR)
    lidt (%esp)

    # Clean up the stack and return
    addl $6, %esp
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for i386_lidt."
#endif
