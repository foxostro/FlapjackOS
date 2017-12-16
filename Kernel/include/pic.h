#pragma once

#include <stdbool.h>

// Initialize the PIC for use in protected mode.
void pic_init(void);

// Clear the PIC, signaling the end of an interrupt.
// Returns true if it turns out the interrupt was spurious.
// In this case, no interrupt handling should occur.
bool pic_clear(unsigned interrupt_number);
