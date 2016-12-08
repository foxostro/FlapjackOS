#include <stddef.h>
#include <stdint.h>
#include <pic.h>
#include <idt.h>
#include <interrupt_asm.h>
#include <timer.h>
#include <console.h>

static void install_handlers(void)
{
	idt_reset();
	idt_install_trap_gate(TIMER_IDT_ENTRY, asm_timer_wrapper, 0);
}

void kernel_main(void)
{
	pic_init();
	install_handlers();

    console_init((vgachar_t *)0xB8000);
    console_puts("Hello, world!\n");

    timer_init(TIMER_RATE_10ms, TIMER_LEAP_INTERVAL_10ms, TIMER_LEAP_TICKS_10ms);
}
