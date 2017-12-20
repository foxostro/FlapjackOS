// Abstract interface for a VGA console output driver.
#pragma once

#include <cstddef>
#include <cstdint>

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

constexpr unsigned CONSOLE_WIDTH  = 80;
constexpr unsigned CONSOLE_HEIGHT = 25;

class console_device {
public:
    virtual ~console_device() = default;

    // Clears the console to the current background color.
    virtual void clear() = 0;

    // Draws the specified character at the specified position in the console buffer.
    virtual void draw_char(size_t row, size_t col, vgachar_t ch) = 0;

    // Gets the character at the specified position of the console buffer.
    virtual vgachar_t get_char(size_t row, size_t col) const = 0;

    // Returns a VGA character in the current color for the specified ASCII char.
    virtual vgachar_t make_char(char ch) const = 0;

    // Returns true if the console can accept the given character for printing.
    // This includes so-called isprint characters as well as characters like
    // BACKSPACE and NEWLINE which can also affect console output.
    virtual bool is_acceptable(char ch) const = 0;

    // Puts a character at the next place on the console.
    virtual void putchar(char ch) = 0;

    // Puts the string at the next position on the console.
    virtual void puts(const char *s) = 0;

    // Moves the hardware cursor to the specified position.
    // If the cursor is placed outside the visible console then it will be hidden.
    virtual void set_cursor_position(size_t row, size_t col) = 0;

    virtual size_t get_cursor_row() const = 0;
    virtual size_t get_cursor_col() const = 0;
};