#include <stddef.h>
#include <idt_asm.h>

idt_entry_t g_idt[IDT_MAX];

idt_entry_t* idt_base(void)
{
    return g_idt;
}

void lidt(__attribute__((unused)) void *idt, __attribute__((unused)) unsigned limit)
{
    // do nothing
}
