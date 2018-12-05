#include <seg.h>

.global i386_ltr
i386_ltr:
#ifdef __i386__
    movl $SEGSEL_TSS, %eax
    ltr %ax
    ret
#elif __x86_64__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for i386_ltr."
#endif