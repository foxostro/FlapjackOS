// Abstract interface for a VGA console output driver.
#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

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
 
typedef struct {
    uint16_t ch:8;
    uint16_t fg:4;
    uint16_t bg:3;
    uint16_t blink:1;
} vgachar_t;
_Static_assert(sizeof(vgachar_t) == 2, "Characters in the VGA text buffer are 2 bytes.");

#define CONSOLE_WIDTH   (80)
#define CONSOLE_HEIGHT  (25)

typedef struct {
    // Initializes and clears the console.
    // addr -- The address of the VGA text buffer.
    void (*init)(vgachar_t * const addr);

    // Clears the console to the current background color.
    void (*clear)(void);

    // Draws the specified character at the specified position in the console buffer.
    void (*draw_char)(size_t row, size_t col, vgachar_t ch);

    // Gets the character at the specified position of the console buffer.
    vgachar_t (*get_char)(size_t row, size_t col);

    // Returns a VGA character in the current color for the specified ASCII char.
    vgachar_t (*make_char)(char ch);

    // Returns true if the console can accept the given character for printing.
    // This includes so-called isprint characters as well as characters like
    // BACKSPACE and NEWLINE which can also affect console output.
    bool (*is_acceptable)(char ch);

    // Puts a character at the next place on the console.
    void (*putchar)(char ch);

    // Puts the string at the next position on the console.
    void (*puts)(const char *s);

    // Moves the hardware cursor to the specified position.
    // If the cursor is placed outside the visible console then it will be hidden.
    void (*set_cursor_position)(size_t row, size_t col);

    size_t (*get_cursor_row)(void);
    size_t (*get_cursor_col)(void);
} console_interface_t;

#ifdef __cplusplus
}
#endif