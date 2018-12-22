#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_PC_PIC_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_PC_PIC_HPP

#include <cstdint>

// Represents the 8259 Programmable Interrupt Controller on PC systems.
class ProgrammableInterruptController {
public:
    // Initialize the PIC for use in protected mode.
    void init();

    // Clear the PIC, signaling the end of an interrupt.
    // Returns true if it turns out the interrupt was spurious.
    // In this case, no interrupt handling should occur.
    bool clear(unsigned interrupt_number);

private:
    void remap(unsigned char master_base, unsigned char slave_base);
    uint16_t get_isr();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_PC_PIC_HPP
