#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP

namespace x86_64 {

class HardwareInterruptController {
public:
    HardwareInterruptController();
    void init();
    void disable_interrupts();
    void enable_interrupts();
    bool clear(unsigned interrupt_number);
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_HARDWARE_INTERRUPT_CONTROLLER_HPP
