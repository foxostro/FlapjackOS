#pragma once

#include <cstddef>
#include <cstdint>
#include <common/terminal_metrics.hpp>
#include <common/vec2.hpp>

enum vgacolor_t {
    BLACK    = 0x0,
    BLUE     = 0x1,
    GREEN    = 0x2,
    CYAN     = 0x3,
    RED      = 0x4,
    MAGENTA  = 0x5,
    BROWN    = 0x6,
    LGRAY    = 0x7, // Light gray
    DGRAY    = 0x8, // Dark gray
    BBLUE    = 0x9, // Bright blue
    BGREEN   = 0xA, // Bright green
    BCYAN    = 0xB, // Bright cyan
    PINK     = 0xC,
    BMAGENTA = 0xD, // Bright magenta
    YELLOW   = 0xE,
    WHITE    = 0xF
};

struct vgachar_t {
    uint16_t ch:8;
    uint16_t fg:4;
    uint16_t bg:3;
    uint16_t blink:1;
};
static_assert(sizeof(vgachar_t) == 2, "Characters in the VGA text buffer are 2 bytes.");

// A text display device is a physical device which displays a grid of monospace
// text. An example of a console is a VGA display in text mode.
class text_display_device {
public:
    virtual ~text_display_device() = default;

    // Clears the console to the current background color.
    virtual void clear() = 0;

    // Draws the specified character at the specified position in the console buffer.
    virtual void draw_char(point2_t pos, vgachar_t ch) = 0;

    // Gets the character at the specified position of the console buffer.
    virtual vgachar_t get_char(point2_t pos) const = 0;

    // Returns a VGA character in the current color for the specified ASCII char.
    virtual vgachar_t make_char(char ch) const = 0;

    // Moves the hardware cursor to the specified position.
    // If the cursor is placed outside the visible console then it will be hidden.
    // TODO: package the position into a point2_t for set_cursor_position()
    virtual void set_cursor_position(point2_t pos) = 0;

    // Gets the row and column of the display on which the cursor was placed.
    virtual point2_t get_cursor_position() const = 0;
};
