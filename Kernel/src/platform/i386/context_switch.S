# Switch execution to a new thread.
# Implement a function context_switch() with a definition like so:
# void i386_context_switch(uintptr_t *old_stack_pointer,
#                          uintptr_t new_stack_pointer);
.globl i386_context_switch
i386_context_switch:
#ifdef __i386__
    # Setup a stack frame.
    pushl %ebp
    movl %esp, %ebp
    
    pusha
    
    # Return the old stack pointer so we can restore it later.
    movl 8(%ebp), %eax
    movl %esp, (%eax)
    
    # Switch to a new stack.
    movl 12(%ebp), %eax
    movl %eax, %esp
    
    popa
    
    # Tear down the stack frame.
    movl %ebp, %esp
    popl %ebp
    
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for i386_context_switch."
#endif
