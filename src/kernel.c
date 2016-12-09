#include <stddef.h>
#include <stdint.h>
#include <strings.h>
#include <seg.h>
#include <gdt.h>
#include <idt.h>
#include <tss.h>
#include <ltr.h>
#include <interrupt_asm.h>
#include <pic.h>
#include <console.h>
#include <kprintf.h>
#include <timer.h>
#include <inout.h>

#define KEY_IDT_ENTRY 0x21
#define KEYBOARD_PORT 0x60

static gdt_entry_t s_gdt[6];
static tss_struct_t s_tss;
static idt_entry_t s_idt[IDT_ENTS];

void keyboard_int_handler()
{
    uint8_t key = inb(KEYBOARD_PORT);
    kprintf("key press: %d\n", key);
    pic_clear();
}

__attribute__((noreturn))
void kernel_main(void *mb_info, void *istack)
{
    // Setup the initial Task State Segment. The kernel uses one TSS between all
    // tasks and performs software task switching.
    bzero(&s_tss, sizeof(s_tss));
    s_tss.ss0 = SEGSEL_KERNEL_DS;
    s_tss.esp0 = (uint32_t)istack;
    s_tss.iomap = sizeof(s_tss);

    // Setup the Global Descriptor Table. The kernel uses a flat memory map
    // and so the GDT only requires five entries:
    // 1 -- descriptor for the TSS
    // 2 -- descriptor for kernel code
    // 3 -- descriptor for kernel data
    // 4 -- descriptor for user space code
    // 5 -- descriptor for user space data
    bzero(s_gdt, sizeof(s_gdt));
    gdt_create_flat_mapping(s_gdt, sizeof(s_gdt), (uint32_t)&s_tss);
    lgdt(s_gdt, sizeof(s_gdt) - 1);
    load_task_register();

    // Setup the Interrupt Descriptor Table. This wires various IRQs up to their
    // handler functions.
    bzero(s_idt, sizeof(idt_entry_t) * IDT_ENTS);
    idt_build_entry(&s_idt[KEY_IDT_ENTRY], (uint32_t)asm_keyboard_wrapper, INTERRUPT_GATE, 0);
    idt_build_entry(&s_idt[TIMER_IDT_ENTRY], (uint32_t)asm_timer_wrapper, INTERRUPT_GATE, 0);
    lidt(s_idt, sizeof(s_idt) - 1);

    // Initialize the PIC chip. It is necessary to remap IRQs before enabling
    // interrupts.
    pic_init();

    console_init((vgachar_t *)0xB8000);
    kprintf("Hello, world!\n");
    kprintf("mb_info = %p\n", mb_info);
    kprintf("istack = %p\n", istack);

    // Configure the PIC timer chip so that it fires an interrupt every 10ms.
    timer_init(TIMER_RATE_10ms, TIMER_LEAP_INTERVAL_10ms, TIMER_LEAP_TICKS_10ms);

    // After this point, interrupts will start firing.
    enable_interrupts();

    // Spin forever. After this point, the kernel is entirely driven in response
    // to interrupts.
    for(;;);
    __builtin_unreachable();
}
