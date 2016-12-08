#include <stdint.h>
#include <stddef.h>
#include <console.h>

static vgachar_t *terminal_buffer;

void console_init(vgachar_t * const addr)
{
    terminal_buffer = addr;
    console_clear();
}

void console_clear(void)
{
    for (size_t row = 0; row < TERM_HEIGHT; row++) {
        for (size_t col = 0; col < TERM_WIDTH; col++) {
            console_draw_char(row, col, (vgachar_t){
                .blink = 0,
                .fg = LGRAY,
                .bg = BLACK,
                .ch = ' '
            });
        }
    }
}

void console_draw_char(size_t row, size_t col, vgachar_t ch)
{
    if (row <= TERM_HEIGHT && col <= TERM_WIDTH) {
        const size_t index = row * TERM_WIDTH + col;
        terminal_buffer[index] = ch;
    }
}
