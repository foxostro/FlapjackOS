# Switch execution to a new thread.
# Implement a function context_switch() with a definition like so:
# void x86_64_context_switch(uintptr_t *old_stack_pointer,
#                            uintptr_t new_stack_pointer);
.globl x86_64_context_switch
x86_64_context_switch:
#ifdef __x86_64__
	ud2 # STUB
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_context_switch."
#endif
