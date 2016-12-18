#include <stdbool.h>
#include <console.h>
#include <kprintf.h>
#include <halt.h>
#include <interrupt_asm.h>
#include <backtrace.h>

__attribute__((noreturn))
void panic(const char * restrict fmt, ...)
{
    disable_interrupts();

    char buffer[128];
    va_list args;
    va_start(args, fmt);
    kvsnprintf(buffer, sizeof(buffer), fmt, args);
    va_end(args);

    console_puts("PANIC: ");
    console_puts(buffer);
    console_puts("\n");

    halt_forever();
}

__attribute__((noreturn))
void panic2(const char * restrict message,
            unsigned edi,
            unsigned esi,
            unsigned ebp,
            unsigned esp,
            unsigned ebx,
            unsigned edx,
            unsigned ecx,
            unsigned eax,
            bool error_code_present,
            unsigned error_code,
            unsigned eip)
{    
    kprintf("Registers:\n");
    kprintf("edi = 0x%x\n", edi);
    kprintf("esi = 0x%x\n", esi);
    kprintf("ebp = 0x%x\n", ebp);
    kprintf("esp = 0x%x\n", esp);
    kprintf("ebx = 0x%x\n", ebx);
    kprintf("edx = 0x%x\n", edx);
    kprintf("ecx = 0x%x\n", ecx);
    kprintf("eax = 0x%x\n", eax);
    kprintf("eip = %p\n\n", eip);

    backtrace();

    if (error_code_present) {
        kprintf("Error Code: 0x%x\n", error_code);
    }

    panic("%s", message);
}
