// VGA console output driver.
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include <inout.h>
#include <misc.h>
#include <ctypes.h>
#include <console.h>

#define CRTC_IDX_REG         0x3d4
#define CRTC_DATA_REG        0x3d5
#define CRTC_CURSOR_LSB_IDX  15
#define CRTC_CURSOR_MSB_IDX  14
#define TAB_WIDTH            8

static void console_clear();
static void console_draw_char(size_t row, size_t col, vgachar_t ch);
static void console_set_cursor_position(size_t row, size_t col);
static void console_init(vgachar_t * const addr);

static volatile vgachar_t *s_vga_console_buffer;
static size_t s_cursor_row = 0, s_cursor_col = 0,
              s_curr_fg = LGRAY, s_curr_bg = BLACK;

static inline vgachar_t space_character()
{
    return (vgachar_t){
        .blink = 0,
        .fg = s_curr_fg,
        .bg = s_curr_bg,
        .ch = ' '
    };
}

static void console_init(vgachar_t * const addr)
{
    s_vga_console_buffer = addr;
    console_clear();
}

static void console_clear()
{
    for (size_t row = 0; row < CONSOLE_HEIGHT; row++) {
        for (size_t col = 0; col < CONSOLE_WIDTH; col++) {
            console_draw_char(row, col, space_character());
        }
    }

    s_cursor_row = 0;
    s_cursor_col = 0;

    console_set_cursor_position(0, 0);
}

static void console_draw_char(size_t row, size_t col, vgachar_t ch)
{
    if (row <= CONSOLE_HEIGHT && col <= CONSOLE_WIDTH && isprint(ch.ch)) {
        const size_t index = row * CONSOLE_WIDTH + col;
        s_vga_console_buffer[index] = ch;
    }
}

static vgachar_t console_get_char(size_t row, size_t col)
{
    if (row <= CONSOLE_HEIGHT && col <= CONSOLE_WIDTH) {
        const size_t index = row * CONSOLE_WIDTH + col;
        return s_vga_console_buffer[index];
    } else {
        return space_character();
    }
}

static vgachar_t console_make_char(char ch)
{
    return (vgachar_t){
        .blink = 0,
        .fg = s_curr_fg,
        .bg = s_curr_bg,
        .ch = ch
    };
}

static void console_newline()
{
    if (s_cursor_row == CONSOLE_HEIGHT) {
        for (size_t row = 1; row <= CONSOLE_HEIGHT; row++) {
            for (size_t col = 0; col < CONSOLE_WIDTH; col++) {
                vgachar_t ch = console_get_char(row, col);
                console_draw_char(row - 1, col, ch);
            }
        }
    } else {
        s_cursor_row++;
    }

    s_cursor_col = 0;

    for (size_t col = 0; col < CONSOLE_WIDTH; col++) {
        console_draw_char(s_cursor_row, col, space_character());
    }
}

static void console_tab()
{
    s_cursor_col += TAB_WIDTH - (s_cursor_col % TAB_WIDTH);
}

static void console_backspace()
{
    if (s_cursor_col == 0) {
        s_cursor_col = CONSOLE_WIDTH;

        if (s_cursor_row > 0) {
            s_cursor_row--;
        }
    } else {
        s_cursor_col--;
    }

    console_draw_char(s_cursor_row, s_cursor_col, space_character());
}

static bool console_is_acceptable(char ch)
{
    return isprint(ch) || (ch == '\n') || (ch == '\t') || (ch == '\b');
}

static void console_putchar(char ch)
{
    if (!console_is_acceptable(ch)) {
        return;
    }

    switch (ch) {
        case '\n':
            console_newline();
            break;

        case '\t':
            console_tab();
            break;
            
        case '\b':
            console_backspace();
            break;
            
        default:
            console_draw_char(s_cursor_row, s_cursor_col, (vgachar_t){
                .blink = 0,
                .fg = s_curr_fg,
                .bg = s_curr_bg,
                .ch = ch
            });

            if (s_cursor_col == CONSOLE_WIDTH) {
                console_newline();
            }

            s_cursor_col++;
            break;
    }

    console_set_cursor_position(s_cursor_row, s_cursor_col);
}

static void console_puts(const char *s)
{
    while (*s) {
        console_putchar(*s++);
    }
}

static void console_set_cursor_position(size_t row, size_t col)
{
    unsigned short offset = row*CONSOLE_WIDTH + col;

    // Send the least significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_LSB_IDX);
    outb(CRTC_DATA_REG, WORD_LOWER_BYTE(offset));

    // Send the most significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_MSB_IDX);
    outb(CRTC_DATA_REG, WORD_UPPER_BYTE(offset));
}

static size_t console_get_cursor_row()
{
    return s_cursor_row;
}

static size_t console_get_cursor_col()
{
    return s_cursor_col;
}

void get_console_interface(console_interface_t *interface)
{
    interface->init = console_init;
    interface->clear = console_clear;
    interface->draw_char = console_draw_char;
    interface->get_char = console_get_char;
    interface->make_char = console_make_char;
    interface->is_acceptable = console_is_acceptable;
    interface->putchar = console_putchar;
    interface->puts = console_puts;
    interface->set_cursor_position = console_set_cursor_position;
    interface->get_cursor_row = console_get_cursor_row;
    interface->get_cursor_col = console_get_cursor_col;
}
