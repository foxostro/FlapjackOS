#include <seg.h>

.global x86_64_ltr
x86_64_ltr:
#ifdef __x86_64__
    movl $SEGSEL_TSS, %eax
    ltr %ax
    retq
#elif __i386__
    ud2
#elif __arm__
    udf
#else
#error "Unknown arch. Need a stub for x86_64_ltr."
#endif