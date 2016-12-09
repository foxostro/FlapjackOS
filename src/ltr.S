#include <seg.h>

# Load the one TSS used by the kernel.
# Called once, early in the boot process.
.global load_ltr
load_ltr:
	movl $SEGSEL_TSS, %eax
    ltr %ax
    ret
