# Returns a pointer to the beginning of the installed IDT.
.global idt_base
idt_base:
    # Build a stack frame.
    pushq %rbp
    movq %rsp, %rbp

    # Allocate ten bytes on the stack and store the IDTR there.
    subq $10, %rsp
    sidt (%rsp)

    # Move the eight byte base address from the stack to %rax.
    # We return this value, and we discard the two byte limit.
    popq %rax

    # Clean up the frame and return.
    movq %rbp, %rsp
    popq %rbp
    retq

# Loads the IDTR register.
.global lidt
lidt:
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
