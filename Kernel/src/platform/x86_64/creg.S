.global x86_64_get_cr0
x86_64_get_cr0:
#ifdef __x86_64__
    mov %cr0, %rax
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_get_cr0."
#endif

.global x86_64_set_cr0
x86_64_set_cr0:
#ifdef __x86_64__
    mov %rdi, %cr0 # The first parameter is passed in RDI.
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_set_cr0."
#endif

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

.global x86_64_set_cr3
x86_64_set_cr3:
#ifdef __x86_64__
    mov %rdi, %cr3 # The first parameter is passed in RDI.
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_set_cr3."
#endif

.global x86_64_get_cr4
x86_64_get_cr4:
#ifdef __x86_64__
    mov %cr4, %rax
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_get_cr4."
#endif

.global x86_64_set_cr4
x86_64_set_cr4:
#ifdef __x86_64__
    mov %rdi, %cr4 # The first parameter is passed in RDI.
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_set_cr4."
#endif