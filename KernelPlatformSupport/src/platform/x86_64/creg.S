.global x86_64_get_cr2
x86_64_get_cr2:
#ifdef __x86_64__
    mov %cr2, %rax
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_get_cr2."
#endif

.global x86_64_get_cr3
x86_64_get_cr3:
#ifdef __x86_64__
    mov %cr3, %rax
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_get_cr3."
#endif