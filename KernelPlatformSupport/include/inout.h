#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

// Reads 1 byte from given port.
uint8_t inb(uint16_t port);

// Reads 2 bytes from given port.
uint16_t inw(uint16_t port);

// Reads 4 bytes from given port.
uint32_t ind(uint16_t port);

// Writes 1 byte to target port.
void outb(uint16_t port, uint8_t val);

// Writes 2 bytes to target port.
void outw(uint16_t port, uint16_t val);

// Writes 4 bytes to target port.
void outd(uint16_t port, uint32_t val);

#ifdef __cplusplus
}
#endif