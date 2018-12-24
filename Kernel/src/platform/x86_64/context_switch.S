# Switch execution to a new thread.
# Implement a function context_switch() with a definition like so:
# void x86_64_context_switch(uintptr_t *old_stack_pointer,
#                            uintptr_t new_stack_pointer);
.globl x86_64_context_switch
x86_64_context_switch:
#ifdef __x86_64__
    # Setup a stack frame.
    pushq %rbp
    movq %rsp, %rbp
    
    pushq %rax
    pushq %rcx
    pushq %rdx
    pushq %rbx
    pushq %rsp
    pushq %rbp
    pushq %rsi
    pushq %rdi
    pushq %r8
    pushq %r9
    pushq %r10
    pushq %r11
    pushq %r12
    pushq %r13
    pushq %r14
    pushq %r15
    
    # Return the old stack pointer so we can restore it later.
    # (First parameter)
    movq %rsp, (%rdi)
    
    # Switch to a new stack.
    # (Second parameter)
    movq %rsi, %rsp
    
    popq %r15
    popq %r14
    popq %r13
    popq %r12
    popq %r11
    popq %r10
    popq %r9
    popq %r8
    popq %rdi
    popq %rsi
    popq %rbp
    popq %rsp
    popq %rbx
    popq %rdx
    popq %rcx
    popq %rax
    
    # Tear down the stack frame.
    movq %rbp, %rsp
    popq %rbp
    
    ret
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_context_switch."
#endif
