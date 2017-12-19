#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// Halt the machine forever
__attribute__((noreturn))
void halt_forever(void);

// Execute a halt instruction to halt the CPU until an interrupt arrives.
// If interrupts are disabled then this will take a very long time.
void hlt(void);

#ifdef __cplusplus
}
#endif