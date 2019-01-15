#include <inout.h>
#include <drivers/pc/serial.hpp>

Serial::Serial()
{
	outb(COM1 + 1, 0x00);   // Turn off interrupts
	outb(COM1 + 3, 0x80);   // SET DLAB ON
	outb(COM1 + 0, 0x01);   // Set Baud rate - Divisor Latch Low Byte
	                        // Default 0x03 =  38,400 BPS
	                        //         0x01 = 115,200 BPS
	                        //         0x02 =  57,600 BPS
	                        //         0x06 =  19,200 BPS
	                        //         0x0C =   9,600 BPS
	                        //         0x18 =   4,800 BPS
	                        //         0x30 =   2,400 BPS
	outb(COM1 + 1, 0x00);   // Set Baud rate - Divisor Latch High Byte
	outb(COM1 + 3, 0x03);   // 8 Bits, No Parity, 1 Stop Bit
	outb(COM1 + 2, 0xC7);   // FIFO Control Register
	outb(COM1 + 4, 0x0B);   // Turn on DTR, RTS, and OUT2

	putchar(0); // First character keeps getting swallows for some reason?
}

void Serial::puts(const char* s)
{
    while (*s) {
        putchar(*s++);
    }
    putchar('\n');
}

void Serial::putchar(char ch)
{
	outb(COM1, ch);
	pause();
}

void Serial::pause()
{
	for (volatile int countdown = 100; countdown >= 0; --countdown);
}