#pragma once

// Initialize the PIC for use in protected mode.
void pic_init();

// Clear the PIC, signaling the end of an interrupt.
void pic_clear();
