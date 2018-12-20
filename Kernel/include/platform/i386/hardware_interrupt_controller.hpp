#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP

#include "idt.hpp"

namespace i386 {

class HardwareInterruptController {
public:
    HardwareInterruptController();
    void init();
    void disable_interrupts();
    void enable_interrupts();
    bool clear(unsigned interrupt_number);

private:
    InterruptDescriptorTable idt_;

    void build_idt();
    void build_idt_entry(InterruptDescriptor& entry, uintptr_t offset);
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP
