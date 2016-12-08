#include <stdint.h>
#include <stddef.h>
#include <console.h>
#include <asm.h>
#include <misc.h>

static const uint16_t CRTC_IDX_REG = 0x3d4;
static const uint16_t CRTC_DATA_REG = 0x3d5;
static const uint8_t CRTC_CURSOR_LSB_IDX = 15;
static const uint8_t CRTC_CURSOR_MSB_IDX = 14;

static vgachar_t *terminal_buffer;
static size_t cursor_row = 0, cursor_col = 0,
              curr_fg = LGRAY, curr_bg = BLACK;

static inline vgachar_t space_character()
{
    return (vgachar_t){
        .blink = 0,
        .fg = curr_fg,
        .bg = curr_bg,
        .ch = ' '
    };
}

bool isprint(int c)
{
    return ((c) >= ' ') && ((c) <= 126);
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

    console_next_cursor_position();
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
    if (row <= TERM_HEIGHT && col <= TERM_WIDTH && isprint(ch.ch)) {
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

    console_next_cursor_position();
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

    console_next_cursor_position();
}

void console_puts(const char *s)
{
    while(*s)
    {
        console_putchar(*s++);
    }
}

void console_set_hardware_cursor_position(int row, int col)
{
    unsigned short offset = row*TERM_WIDTH + col;

    // Send the least significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_LSB_IDX);
    outb(CRTC_DATA_REG, WORD_LOWER_BYTE(offset));

    // Send the most significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_MSB_IDX);
    outb(CRTC_DATA_REG, WORD_UPPER_BYTE(offset));
}

void console_next_cursor_position()
{
    if (cursor_col == TERM_WIDTH) {
        console_set_hardware_cursor_position(cursor_row + 1, 0);
    } else {
        console_set_hardware_cursor_position(cursor_row, cursor_col + 1);
    }
}
