.global i386_get_cr0
i386_get_cr0:
#ifdef __i386__
    movl %cr0, %eax
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for i386_get_cr0."
#endif

.global i386_set_cr0
i386_set_cr0:
#ifdef __i386__
    movl 4(%esp), %eax
    mov %eax, %cr0
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_set_cr0."
#endif

.global i386_get_cr2
i386_get_cr2:
#ifdef __i386__
    movl %cr2, %eax
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for i386_get_cr2."
#endif

.global i386_get_cr3
i386_get_cr3:
#ifdef __i386__
    movl %cr3, %eax
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for i386_get_cr3."
#endif

.global i386_set_cr3
i386_set_cr3:
#ifdef __i386__
    movl 4(%esp), %eax
    mov %eax, %cr3
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_set_cr3."
#endif

.global i386_get_cr4
i386_get_cr4:
#ifdef __i386__
    movl %cr4, %eax
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for i386_get_cr4."
#endif

.global i386_set_cr4
i386_set_cr4:
#ifdef __i386__
    movl 4(%esp), %eax
    mov %eax, %cr4
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_set_cr4."
#endif