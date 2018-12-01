#include <vga_text_display_device.hpp>

#include <kernel_image_info.hpp> // for KERNEL_VIRTUAL_START_ADDR
#include <inout.h>
#include <common/misc.h>

#include <cstdint>
#include <cstddef>
#include <cstring>
#include <cctype>

constexpr unsigned CRTC_IDX_REG         = 0x3d4;
constexpr unsigned CRTC_DATA_REG        = 0x3d5;
constexpr unsigned CRTC_CURSOR_LSB_IDX  = 15;
constexpr unsigned CRTC_CURSOR_MSB_IDX  = 14;

volatile uint16_t *g_vga_text_buffer = (uint16_t *)(0xB8000 + KERNEL_VIRTUAL_START_ADDR);
static_assert(sizeof(VGAChar) == sizeof(uint16_t));

inline VGAChar vga_text_buffer_get(size_t index)
{
    uint16_t value = g_vga_text_buffer[index];
    return VGAChar(value);
}

inline void vga_text_buffer_set(VGAChar ch, size_t index)
{
    g_vga_text_buffer[index] = ch.get_value();
}

VGATextDisplayDevice::VGATextDisplayDevice()
 : cursor_pos_{0,0},
   current_foreground_color_(LGRAY),
   current_background_color_(BLACK)
{}

void VGATextDisplayDevice::clear()
{
    for (Point2 pos{0,0}; pos.y < CONSOLE_HEIGHT; pos.y++) {
        for (pos.x = 0; pos.x < CONSOLE_WIDTH; pos.x++) {
            draw_char(pos, make_char(' '));
        }
    }

    set_cursor_position({0, 0});
}

void VGATextDisplayDevice::draw_char(Point2 pos, VGAChar ch)
{
    if (pos.x >= 0 && pos.y >= 0 && pos.y < CONSOLE_HEIGHT && pos.x < CONSOLE_WIDTH && isprint(ch.attr.ch)) {
        const size_t index = pos.y * CONSOLE_WIDTH + pos.x;
        vga_text_buffer_set(ch, index);
    }
}

VGAChar VGATextDisplayDevice::get_char(Point2 pos) const
{
    if (pos.x >= 0 && pos.y >= 0 && pos.y < CONSOLE_HEIGHT && pos.x < CONSOLE_WIDTH) {
        const size_t index = pos.y * CONSOLE_WIDTH + pos.x;
        return vga_text_buffer_get(index);
    } else {
        return space_character();
    }
}

VGAChar VGATextDisplayDevice::make_char(char ch) const
{
    VGAChar r;
    r.attr.blink = 0;
    r.attr.fg = current_foreground_color_;
    r.attr.bg = current_background_color_;
    r.attr.ch = ch;
    return r;
}

void VGATextDisplayDevice::set_cursor_position(Point2 pos)
{
    cursor_pos_ = pos;

    unsigned short offset = pos.y*CONSOLE_WIDTH + pos.x;

    // Send the least significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_LSB_IDX);
    outb(CRTC_DATA_REG, WORD_LOWER_BYTE(offset));

    // Send the most significant byte of the offset.
    outb(CRTC_IDX_REG, CRTC_CURSOR_MSB_IDX);
    outb(CRTC_DATA_REG, WORD_UPPER_BYTE(offset));
}

Point2 VGATextDisplayDevice::get_cursor_position() const
{
    return cursor_pos_;
}

size_t VGATextDisplayDevice::get_tab_width() const
{
    return TAB_WIDTH;
}

Size2 VGATextDisplayDevice::dimensions() const
{
    return {CONSOLE_WIDTH, CONSOLE_HEIGHT};
}
