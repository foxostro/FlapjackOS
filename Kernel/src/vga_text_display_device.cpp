#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cctype>

#include <inout.h>
#include <misc.h>

#include <vga_text_display_device.hpp>

constexpr unsigned CRTC_IDX_REG         = 0x3d4;
constexpr unsigned CRTC_DATA_REG        = 0x3d5;
constexpr unsigned CRTC_CURSOR_LSB_IDX  = 15;
constexpr unsigned CRTC_CURSOR_MSB_IDX  = 14;

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

vga_text_display_device::vga_text_display_device()
 : cursor_pos{0,0},
   curr_fg(LGRAY),
   curr_bg(BLACK)
{}

void vga_text_display_device::clear()
{
    for (point2_t pos{0,0}; pos.y < CONSOLE_HEIGHT; pos.y++) {
        for (pos.x = 0; pos.x < CONSOLE_WIDTH; pos.x++) {
            draw_char(pos, make_char(' '));
        }
    }

    set_cursor_position({0, 0});
}

void vga_text_display_device::draw_char(point2_t pos, vgachar_t ch)
{
    if (pos.x >= 0 && pos.y >= 0 && pos.y < CONSOLE_HEIGHT && pos.x < CONSOLE_WIDTH && isprint(ch.ch)) {
        const size_t index = pos.y * CONSOLE_WIDTH + pos.x;
        vga_text_buffer_set(ch, index);
    }
}

vgachar_t vga_text_display_device::get_char(point2_t pos) const
{
    if (pos.x >= 0 && pos.y >= 0 && pos.y < CONSOLE_HEIGHT && pos.x < CONSOLE_WIDTH) {
        const size_t index = pos.y * CONSOLE_WIDTH + pos.x;
        return vga_text_buffer_get(index);
    } else {
        return space_character();
    }
}

vgachar_t vga_text_display_device::make_char(char ch) const
{
    vgachar_t r;
    r.blink = 0;
    r.fg = curr_fg;
    r.bg = curr_bg;
    r.ch = ch;
    return r;
}

void vga_text_display_device::set_cursor_position(point2_t pos)
{
    cursor_pos = pos;

    unsigned short offset = pos.y*CONSOLE_WIDTH + pos.x;

    // Send the least significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_LSB_IDX);
    outb(CRTC_DATA_REG, WORD_LOWER_BYTE(offset));

    // Send the most significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_MSB_IDX);
    outb(CRTC_DATA_REG, WORD_UPPER_BYTE(offset));
}

point2_t vga_text_display_device::get_cursor_position() const
{
    return cursor_pos;
}

size_t vga_text_display_device::get_tab_width() const
{
    return TAB_WIDTH;
}

size2_t vga_text_display_device::dimensions() const
{
    return {CONSOLE_WIDTH, CONSOLE_HEIGHT};
}
