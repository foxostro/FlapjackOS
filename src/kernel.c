#include <stddef.h>
#include <stdint.h>
#include <console.h>

void kernel_main(void)
{
    console_init((vgachar_t *)0xB8000);
    console_draw_char(0, 0, (vgachar_t){
        .blink = 0,
        .fg = LGRAY,
        .bg = BLACK,
        .ch = 'H'
    });
    console_draw_char(0, 1, (vgachar_t){
        .blink = 0,
        .fg = LGRAY,
        .bg = BLACK,
        .ch = 'i'
    });
}
