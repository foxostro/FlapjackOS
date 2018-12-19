#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_FAULT_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_FAULT_INTERRUPT_HANDLER_HPP

#include "generic_interrupt_handler.hpp"
#include "interrupt_parameters.hpp"

namespace x86_64 {

class PageFaultInterruptHandler : public GenericInterruptHandler<InterruptParameters> {
public:
    PageFaultInterruptHandler();

    void int_handler(const InterruptParameters& params) noexcept override;
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_PAGE_FAULT_INTERRUPT_HANDLER_HPP
