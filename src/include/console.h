#pragma once

#include <stddef.h>
#include <stdint.h>

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

static const size_t TERM_WIDTH = 80;
static const size_t TERM_HEIGHT = 25;

// Initializes and clears the console.
// addr -- The address of the VGA text buffer.
void console_init(vgachar_t * const addr);

// Clears the console to the current background color.
void console_clear(void);

// Draws the specified character at the specified position in the terminal buffer.
void console_draw_char(size_t row, size_t col, vgachar_t ch);

// Gets the character at the specified position of the terminal buffer.
vgachar_t console_get_char(size_t row, size_t col);

// Puts a character at the next place in the terminal.
void console_putchar(char ch);

// Puts the string at the next position in the termoinal.
void console_puts(const char *s);

// Sets the hardware cursor at the specified position.
// If the cursor is placed outside the visible console then it will be hidden.
void console_set_hardware_cursor_position(int row, int col);

// Move the cursor to the next position where a character will be inserted.
void console_next_cursor_position();
