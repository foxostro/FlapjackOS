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
	uint16_t character:8;
	uint16_t fg:4;
	uint16_t bg:3;
	uint16_t blink:1;
} vgachar_t;
_Static_assert(sizeof(vgachar_t) == 2, "Character in the VGA text buffer are 2 bytes.");
 
static const size_t TERM_WIDTH = 80;
static const size_t TERM_HEIGHT = 25;
static vgachar_t * const terminalBuffer = (vgachar_t *)0xB8000;

void kernelMain(void)
{
	for (size_t y = 0; y < TERM_HEIGHT; y++) {
		for (size_t x = 0; x < TERM_WIDTH; x++) {
			const size_t index = y * TERM_WIDTH + x;
			terminalBuffer[index] = (vgachar_t){
				.blink = 0,
				.fg = LGRAY,
				.bg = BLACK,
				.character = 'c'
			};
		}
	}
}
