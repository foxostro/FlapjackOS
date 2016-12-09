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
#include <halt.h>
#include <panic.h>
#include <backtrace.h>

#define KEYBOARD_PORT 0x60

static gdt_entry_t s_gdt[6];
static tss_struct_t s_tss;
static idt_entry_t s_idt[IDT_ENTS];

void keyboard_int_handler()
{
    int key = inb(KEYBOARD_PORT);
    kprintf("key press: %d (time=%u)\n", key, timer_seconds());
}

void interrupt_dispatch(unsigned interrupt_number,
                        __attribute__((unused)) unsigned edi,
                        __attribute__((unused)) unsigned esi,
                        __attribute__((unused)) unsigned ebp,
                        __attribute__((unused)) unsigned esp,
                        __attribute__((unused)) unsigned ebx,
                        __attribute__((unused)) unsigned edx,
                        __attribute__((unused)) unsigned ecx,
                        __attribute__((unused)) unsigned eax,
                        unsigned error_code)
{
    switch(interrupt_number) {
        case KEY_IDT_ENTRY:
            keyboard_int_handler();
            break;

        case TIMER_IDT_ENTRY:
            timer_int_handler();
            break;

        case IDT_DE:
            backtrace();
            panic("Division Error");
            break;

        case IDT_DB:
            backtrace();
            panic("Debug Exception");
            break;

        case IDT_NMI:
            backtrace();
            panic("Non-Maskable Interrupt");
            break;

        case IDT_BP:
            backtrace();
            panic("Breakpoint");
            break;

        case IDT_OF:
            backtrace();
            panic("Overflow");
            break;

        case IDT_BR:
            backtrace();
            panic("BOUND Range exceeded");
            break;

        case IDT_UD:
            backtrace();
            panic("UnDefined Opcode");
            break;

        case IDT_NM:
            backtrace();
            panic("No Math coprocessor");
            break;

        case IDT_DF:
            backtrace();
            panic("Double Fault. Error Code: 0x%x", error_code);
            break;

        case IDT_CSO:
            backtrace();
            panic("Coprocessor Segment Overrun");
            break;

        case IDT_TS:
            backtrace();
            panic("Invalid Task Segment Selector. Error Code: 0x%x", error_code);
            break;

        case IDT_NP:
            backtrace();
            panic("Segment Not Present. Error Code: 0x%x", error_code);
            break;

        case IDT_SS:
            backtrace();
            panic("Stack Segment Fault. Error Code: 0x%x", error_code);
            break;

        case IDT_GP:
            backtrace();
            panic("General Protection Fault. Error Code: 0x%x", error_code);
            break;

        case IDT_PF:
            backtrace();
            panic("Page Fault. Error Code: 0x%x", error_code);
            break;

        case IDT_MF:
            backtrace();
            panic("X87 Math Fault");
            break;

        case IDT_AC:
            backtrace();
            panic("Alignment Check");
            break;

        case IDT_MC:
            backtrace();
            panic("Machine Check");
            break;

        case IDT_XF:
            backtrace();
            panic("SSE Floating Point Exception");
            break;

        default:
            backtrace();
            panic("Unknown interrupt: %d", interrupt_number);
    }

    pic_clear(interrupt_number);
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
    idt_build_entry(&s_idt[IDT_DE],          (uint32_t)asm_de_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_DB],          (uint32_t)asm_db_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_NMI],         (uint32_t)asm_nmi_wrapper,      INTERRUPT_GATE, 0);
    idt_build_entry(&s_idt[IDT_BP],          (uint32_t)asm_bp_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_OF],          (uint32_t)asm_of_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_BR],          (uint32_t)asm_br_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_UD],          (uint32_t)asm_ud_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_NM],          (uint32_t)asm_nm_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_DF],          (uint32_t)asm_df_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_CSO],         (uint32_t)asm_cso_wrapper,      TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_TS],          (uint32_t)asm_ts_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_NP],          (uint32_t)asm_np_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_SS],          (uint32_t)asm_ss_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_GP],          (uint32_t)asm_gp_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_PF],          (uint32_t)asm_pf_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_MF],          (uint32_t)asm_mf_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_AC],          (uint32_t)asm_ac_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_MC],          (uint32_t)asm_mc_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[IDT_XF],          (uint32_t)asm_xf_wrapper,       TRAP_GATE, 0);
    idt_build_entry(&s_idt[KEY_IDT_ENTRY],   (uint32_t)asm_keyboard_wrapper, INTERRUPT_GATE, 0);
    idt_build_entry(&s_idt[TIMER_IDT_ENTRY], (uint32_t)asm_timer_wrapper,    INTERRUPT_GATE, 0);
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
    halt();
    __builtin_unreachable();
}
