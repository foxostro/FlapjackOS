#include <stddef.h>
#include <stdint.h>
 
static const size_t TERM_WIDTH = 80;
static const size_t TERM_HEIGHT = 25;
static uint16_t * const terminal_buffer = (uint16_t*)0xB8000;
 
void kernel_main(void)
{
	uint16_t fg = 4;
	uint16_t bg = 4;
	uint16_t terminal_color = fg | bg << 4;

	// Fill the terminal with red.
	for (size_t y = 0; y < TERM_HEIGHT; y++)
	{
		for (size_t x = 0; x < TERM_WIDTH; x++)
		{
			const size_t index = y * TERM_WIDTH + x;
			terminal_buffer[index] = (uint16_t)' ' | terminal_color << 8;
		}
	}
}
