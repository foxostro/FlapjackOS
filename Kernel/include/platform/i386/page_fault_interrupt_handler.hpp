#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FAULT_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FAULT_INTERRUPT_HANDLER_HPP

#include "generic_interrupt_handler.hpp"
#include "interrupt_parameters.hpp"

namespace i386 {

class PageFaultInterruptHandler : public GenericInterruptHandler<InterruptParameters> {
public:
    PageFaultInterruptHandler();

    void int_handler(const InterruptParameters& params) override;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_PAGE_FAULT_INTERRUPT_HANDLER_HPP
