#include <stddef.h>
#include <stdint.h>
#include <console.h>

void kernel_main(void)
{
    console_init((vgachar_t *)0xB8000);
    console_puts("Hello, world!\n");
}
