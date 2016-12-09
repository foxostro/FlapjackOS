.global asm_timer_wrapper
asm_timer_wrapper:
    cli
    pusha
    call timer_int_handler
    popa
    iret

.global asm_keyboard_wrapper
asm_keyboard_wrapper:
    cli
    pusha
    call keyboard_int_handler
    popa
    iret

.global enable_interrupts
enable_interrupts:
	sti
	ret

.global disable_interrupts
disable_interrupts:
    cli
    ret
