#ifndef FLAPJACKOS_KERNEL_INCLUDE_PANIC_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PANIC_INTERRUPT_HANDLER_HPP

#include <panic.h>
#include <cstring>
#include "generic_interrupt_handler.hpp"
#include <platform/i386/interrupt_parameters.hpp> // AFOX_TODO: PanicInterruptHandler needs to be made general enough to work on i386 and also x86_64.

class PanicInterruptHandler : public GenericInterruptHandler<i386::InterruptParameters> {
public:
    PanicInterruptHandler(const char *message, bool error_code_present);

    using ParameterPackage = i386::InterruptParameters;
    void int_handler(const ParameterPackage& params) noexcept override;

private:
    char message_[64];
    bool error_code_present_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PANIC_INTERRUPT_HANDLER_HPP
