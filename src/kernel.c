#include <stddef.h>
#include <stdint.h>
#include <strings.h>
#include <gdt.h>
#include <idt.h>
#include <interrupt_asm.h>
#include <pic.h>
#include <console.h>
#include <timer.h>

#define KEY_IDT_ENTRY 0x21

extern void* init_tss; // defined in boot.S
void load_task_register(); // defined in ltr.S

static gdt_entry_t s_gdt[6];
static idt_entry_t s_idt[IDT_ENTS];

void keyboard_int_handler()
{
    console_puts("key press\n");
    pic_clear();
}

void kernel_main()
{
    gdt_create_flat_mapping(s_gdt, sizeof(s_gdt), (uint32_t)init_tss);
    lgdt(s_gdt, sizeof(s_gdt) - 1);

    bzero(s_idt, sizeof(idt_entry_t) * IDT_ENTS);
    idt_build_entry(&s_idt[KEY_IDT_ENTRY], (uint32_t)asm_keyboard_wrapper, INTERRUPT_GATE, 0);
    idt_build_entry(&s_idt[TIMER_IDT_ENTRY], (uint32_t)asm_timer_wrapper, INTERRUPT_GATE, 0);
    lidt(s_idt, sizeof(s_idt) - 1);

    load_task_register();

    pic_init();
    enable_interrupts();

    console_init((vgachar_t *)0xB8000);
    console_puts("Hello, world!\n");

    timer_init(TIMER_RATE_10ms, TIMER_LEAP_INTERVAL_10ms, TIMER_LEAP_TICKS_10ms);
}
