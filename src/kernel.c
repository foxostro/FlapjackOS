#include <stddef.h>
#include <stdint.h>
#include <strings.h>
#include <seg.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <interrupt_asm.h>
#include <pic.h>
#include <console.h>
#include <kprintf.h>
#include <timer.h>

#define KEY_IDT_ENTRY 0x21

void load_task_register(); // defined in ltr.S

static gdt_entry_t s_gdt[6];
static tss_struct_t s_tss;
static idt_entry_t s_idt[IDT_ENTS];

void keyboard_int_handler()
{
    console_puts("key press\n");
    pic_clear();
}

void kernel_main(void *mb_info, void *istack)
{
    bzero(&s_tss, sizeof(s_tss));
    s_tss.ss0 = SEGSEL_KERNEL_DS;
    s_tss.esp0 = (uint32_t)istack;
    s_tss.iomap = sizeof(s_tss);

    bzero(s_gdt, sizeof(s_gdt));
    gdt_create_flat_mapping(s_gdt, sizeof(s_gdt), (uint32_t)&s_tss);
    lgdt(s_gdt, sizeof(s_gdt) - 1);
    load_task_register();

    bzero(s_idt, sizeof(idt_entry_t) * IDT_ENTS);
    idt_build_entry(&s_idt[KEY_IDT_ENTRY], (uint32_t)asm_keyboard_wrapper, INTERRUPT_GATE, 0);
    idt_build_entry(&s_idt[TIMER_IDT_ENTRY], (uint32_t)asm_timer_wrapper, INTERRUPT_GATE, 0);
    lidt(s_idt, sizeof(s_idt) - 1);

    pic_init();

    console_init((vgachar_t *)0xB8000);
    kprintf("Hello, world!\n");
    kprintf("mb_info = %p\n", mb_info);
    kprintf("istack = %p\n", istack);

    timer_init(TIMER_RATE_10ms, TIMER_LEAP_INTERVAL_10ms, TIMER_LEAP_TICKS_10ms);

    enable_interrupts();

    for(;;);
}
