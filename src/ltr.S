#include <seg.h>

# Load the one TSS used by the kernel.
# Called once, early in the boot process.
.global load_task_register
load_task_register:
	movl $SEGSEL_TSS, %eax
    ltr %ax
    ret
