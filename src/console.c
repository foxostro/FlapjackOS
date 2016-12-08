#include <stdint.h>
#include <stddef.h>
#include <console.h>

static vgachar_t *terminal_buffer;
size_t cursor_row = 0, cursor_col = 0;
size_t curr_fg = LGRAY, curr_bg = BLACK; 

static inline vgachar_t space_character()
{
    return (vgachar_t){
        .blink = 0,
        .fg = curr_fg,
        .bg = curr_bg,
        .ch = ' '
    };
}

void console_init(vgachar_t * const addr)
{
    terminal_buffer = addr;
    console_clear();
}

void console_clear(void)
{
    for (size_t row = 0; row < TERM_HEIGHT; row++) {
        for (size_t col = 0; col < TERM_WIDTH; col++) {
            console_draw_char(row, col, space_character());
        }
    }

    cursor_row = 0;
    cursor_col = 0;
}

vgachar_t console_get_char(size_t row, size_t col)
{
    if (row <= TERM_HEIGHT && col <= TERM_WIDTH) {
        const size_t index = row * TERM_WIDTH + col;
        return terminal_buffer[index];
    } else {
        return space_character();
    }
}

void console_draw_char(size_t row, size_t col, vgachar_t ch)
{
    if (row <= TERM_HEIGHT && col <= TERM_WIDTH) {
        const size_t index = row * TERM_WIDTH + col;
        terminal_buffer[index] = ch;
    }
}

void console_newline()
{
    if (cursor_row == TERM_HEIGHT) {
        for (size_t row = 1; row <= TERM_HEIGHT; row++) {
            for (size_t col = 0; col < TERM_WIDTH; col++) {
                vgachar_t ch = console_get_char(row, col);
                console_draw_char(row - 1, col, ch);
            }
        }
    } else {
        cursor_row++;
    }

    cursor_col = 0;
}

void console_putchar(char ch)
{
    if (ch == '\n') {
        console_newline();
        return;
    }

    if (cursor_col == TERM_WIDTH) {
        console_newline();
    }

    cursor_col++;

    console_draw_char(cursor_row, cursor_col, (vgachar_t){
        .blink = 0,
        .fg = curr_fg,
        .bg = curr_bg,
        .ch = ch
    });
}

void console_puts(const char *s)
{
    while(*s)
    {
        console_putchar(*s++);
    }
}
