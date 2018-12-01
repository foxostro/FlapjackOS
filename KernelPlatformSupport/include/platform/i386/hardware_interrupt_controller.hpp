#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP

#include "idt.h"

namespace i386 {

class HardwareInterruptController {
public:
    HardwareInterruptController();
    void init();
    void disable_interrupts();
    void enable_interrupts();

private:
    IDTEntry idt_[IDT_MAX];
};

} // namespace i386

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_PLATFORM_I386_HARDWARE_INTERRUPT_CONTROLLER_HPP