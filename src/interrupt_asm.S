// When a hardware interrupt occurs that we do not care about, this is
// called. It does nothing and returns immediately.
.globl unused_hardware_interrupt;
unused_hardware_interrupt:
    iret

.global asm_timer_wrapper
asm_timer_wrapper:
    pusha
    call timer_int_handler
    popa
    add $4, %esp // Cleans up the error code.
    iret
