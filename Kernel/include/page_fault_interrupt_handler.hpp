#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FAULT_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FAULT_INTERRUPT_HANDLER_HPP

#include <panic.h>
#include <cstring>
#include "interrupt_handler.hpp"
#include <platform/i386/interrupt_parameters.hpp> // AFOX_TODO: PageFaultInterruptHandler needs to be made general enough to work on i386 and also x86_64.

class PageFaultInterruptHandler : public InterruptHandler<i386::InterruptParameters> {
public:
    PageFaultInterruptHandler();

    using ParameterPackage = i386::InterruptParameters;
    void int_handler(const ParameterPackage& params) noexcept override;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FAULT_INTERRUPT_HANDLER_HPP
