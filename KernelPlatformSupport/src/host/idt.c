#include <stddef.h>
#include <idt_asm.h>

IDTEntry g_idt[IDT_MAX];

IDTEntry* idt_base(void)
{
    return g_idt;
}

void lidt(__attribute__((unused)) void *idt, __attribute__((unused)) unsigned limit)
{
    // do nothing
}
