.global asm_timer_wrapper
asm_timer_wrapper:
    pusha
    call timer_int_handler
    popa
    add $4, %esp # Cleans up the error code.
    iret

.global asm_keyboard_wrapper
asm_keyboard_wrapper:
    pusha
    call keyboard_int_handler
    popa
    iret

.global enable_interrupts
enable_interrupts:
	sti
	ret
