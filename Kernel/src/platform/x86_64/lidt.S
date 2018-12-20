# Loads the IDTR register.
.global x86_64_lidt
x86_64_lidt:
    # Build a ten byte descriptor on the stack using the parameters passed in
    # the %rdi and %rsi registers. The lidt instruction wants the address of a
    # descriptor with an eight byte base and a two byte limit.
    pushq %rdi # Eight byte base address
    pushw %si  # Two byte table limit
    
    # Load the global descriptor table register (GDTR)
    lidt (%rsp)
    
    # Cleanup the stack and return
    addq $10, %rsp
    retq
