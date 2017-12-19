// VGA console output driver.
#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cctype>

#include <inout.h>
#include <misc.h>

#include <vga_console_device.hpp>

constexpr unsigned CRTC_IDX_REG         = 0x3d4;
constexpr unsigned CRTC_DATA_REG        = 0x3d5;
constexpr unsigned CRTC_CURSOR_LSB_IDX  = 15;
constexpr unsigned CRTC_CURSOR_MSB_IDX  = 14;
constexpr unsigned TAB_WIDTH            = 8;

volatile uint16_t *g_vga_text_buffer = (uint16_t *)0xB8000;

inline vgachar_t vga_text_buffer_get(size_t index)
{
    uint16_t value = g_vga_text_buffer[index];
    return *(vgachar_t *)&value;
}

inline void vga_text_buffer_set(vgachar_t ch, size_t index)
{
    uint16_t value = *(uint16_t *)&ch;
    g_vga_text_buffer[index] = value;
}

vga_console_device::vga_console_device()
 : cursor_row(0),
   cursor_col(0),
   curr_fg(LGRAY),
   curr_bg(BLACK)
{}

void vga_console_device::clear()
{
    for (size_t row = 0; row < CONSOLE_HEIGHT; row++) {
        for (size_t col = 0; col < CONSOLE_WIDTH; col++) {
            draw_char(row, col, make_char(' '));
        }
    }

    cursor_row = 0;
    cursor_col = 0;

    set_cursor_position(0, 0);
}

void vga_console_device::draw_char(size_t row, size_t col, vgachar_t ch)
{
    if (row <= CONSOLE_HEIGHT && col <= CONSOLE_WIDTH && isprint(ch.ch)) {
        const size_t index = row * CONSOLE_WIDTH + col;
        vga_text_buffer_set(ch, index);
    }
}

vgachar_t vga_console_device::get_char(size_t row, size_t col) const
{
    if (row <= CONSOLE_HEIGHT && col <= CONSOLE_WIDTH) {
        const size_t index = row * CONSOLE_WIDTH + col;
        return vga_text_buffer_get(index);
    } else {
        return space_character();
    }
}

vgachar_t vga_console_device::make_char(char ch) const
{
    vgachar_t r;
    r.blink = 0;
    r.fg = curr_fg;
    r.bg = curr_bg;
    r.ch = ch;
    return r;
}

void vga_console_device::newline()
{
    if (cursor_row == CONSOLE_HEIGHT-1) {
        for (size_t row = 1; row <= CONSOLE_HEIGHT; row++) {
            for (size_t col = 0; col < CONSOLE_WIDTH; col++) {
                vgachar_t ch = get_char(row, col);
                draw_char(row - 1, col, ch);
            }
        }
    } else {
        cursor_row++;
    }

    cursor_col = 0;

    for (size_t col = 0; col < CONSOLE_WIDTH; col++) {
        draw_char(cursor_row, col, space_character());
    }
}

void vga_console_device::tab()
{
    cursor_col += TAB_WIDTH - (cursor_col % TAB_WIDTH);
}

void vga_console_device::backspace()
{
    if (cursor_col == 0) {
        cursor_col = CONSOLE_WIDTH;

        if (cursor_row > 0) {
            cursor_row--;
        }
    } else {
        cursor_col--;
    }

    draw_char(cursor_row, cursor_col, space_character());
}

bool vga_console_device::is_acceptable(char ch) const
{
    return isprint(ch) || (ch == '\n') || (ch == '\t') || (ch == '\b');
}

void vga_console_device::putchar(char ch)
{
    if (!is_acceptable(ch)) {
        return;
    }

    switch (ch) {
        case '\n':
            newline();
            break;

        case '\t':
            tab();
            break;
            
        case '\b':
            backspace();
            break;
            
        default:
            draw_char(cursor_row, cursor_col, make_char(ch));

            if (cursor_col == CONSOLE_WIDTH) {
                newline();
            }

            cursor_col++;
            break;
    }

    set_cursor_position(cursor_row, cursor_col);
}

void vga_console_device::puts(const char *s)
{
    while (*s) {
        putchar(*s++);
    }
}

void vga_console_device::set_cursor_position(size_t row, size_t col)
{
    unsigned short offset = row*CONSOLE_WIDTH + col;

    // Send the least significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_LSB_IDX);
    outb(CRTC_DATA_REG, WORD_LOWER_BYTE(offset));

    // Send the most significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_MSB_IDX);
    outb(CRTC_DATA_REG, WORD_UPPER_BYTE(offset));
}

size_t vga_console_device::get_cursor_row() const
{
    return cursor_row;
}

size_t vga_console_device::get_cursor_col() const
{
    return cursor_col;
}
