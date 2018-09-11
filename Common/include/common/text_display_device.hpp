#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_DISPLAY_DEVICE_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_DISPLAY_DEVICE_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <common/terminal_metrics.hpp>
#include <common/vec2.hpp>

enum VGAColor {
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

// A single character in the hardware VGA text buffer.
class VGAChar {
public:
    union {
        struct {
            uint16_t ch:8;
            uint16_t fg:4;
            uint16_t bg:3;
            uint16_t blink:1;
        } attr;
        uint16_t value_;
    };

    VGAChar() : value_(0) {}

    explicit VGAChar(uint16_t value)
    {
        value_ = value;
    }

    inline uint16_t get_value() const
    {
        return value_;
    }
};

// A text display device is a physical device which displays a grid of monospace
// text. An example of a console is a VGA display in text mode.
class TextDisplayDevice {
public:
    virtual ~TextDisplayDevice() = default;

    // Clears the console to the current background color.
    virtual void clear() = 0;

    // Draws the specified character at the specified position in the console buffer.
    virtual void draw_char(Point2 pos, VGAChar ch) = 0;

    // Gets the character at the specified position of the console buffer.
    virtual VGAChar get_char(Point2 pos) const = 0;

    // Returns a VGA character in the current color for the specified ASCII char.
    virtual VGAChar make_char(char ch) const = 0;

    // Moves the hardware cursor to the specified position.
    // If the cursor is placed outside the visible console then it will be hidden.
    // TODO: package the position into a Point2 for set_cursor_position()
    virtual void set_cursor_position(Point2 pos) = 0;

    // Gets the row and column of the display on which the cursor was placed.
    virtual Point2 get_cursor_position() const = 0;

    // Gets the number of columns needed to display a TAB on this display.
    virtual size_t get_tab_width() const = 0;

    // Gets the number of rows and columns on this display.
    virtual Size2 dimensions() const = 0;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_DISPLAY_DEVICE_HPP
