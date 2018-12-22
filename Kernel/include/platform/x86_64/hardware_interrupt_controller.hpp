#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP

#include "idt.hpp"
#include <platform/pc/pic.hpp>

namespace x86_64 {

// AFOX_TODO: There's a lot of similarities between the i386 and x86_64 HardwareInterruptController classes. Can these two be consolidated?

class HardwareInterruptController {
public:
    HardwareInterruptController();
    void init();
    void disable_interrupts();
    void enable_interrupts();
    bool clear(unsigned interrupt_number);

private:
    ProgrammableInterruptController pic_;
    InterruptDescriptorTable idt_;

    void build_idt();
    void build_idt_entry(InterruptDescriptor& entry, uintptr_t offset);
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP
