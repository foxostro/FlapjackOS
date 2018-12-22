#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PANIC_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PANIC_INTERRUPT_HANDLER_HPP

#include "generic_interrupt_handler.hpp"
#include "interrupt_parameters.hpp"

namespace x86_64 {

class PanicInterruptHandler : public GenericInterruptHandler<InterruptParameters> {
public:
    PanicInterruptHandler(const char *message, bool error_code_present);

    void int_handler(const InterruptParameters& params) override;

private:
    char message_[64];
    bool error_code_present_;
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PANIC_INTERRUPT_HANDLER_HPP
