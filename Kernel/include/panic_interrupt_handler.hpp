#ifndef FLAPJACKOS_KERNEL_INCLUDE_PANIC_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PANIC_INTERRUPT_HANDLER_HPP

#include <common/interrupt_handler.hpp>
#include <panic.h>
#include <cstring>

class PanicInterruptHandler : public InterruptHandler {
public:
    PanicInterruptHandler(const char *message, bool error_code_present);

    void int_handler(const ParameterPackage& params) override;

private:
    char message_[64];
    bool error_code_present_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PANIC_INTERRUPT_HANDLER_HPP