#ifndef FLAPJACKOS_KERNEL_INCLUDE_PAGE_FAULT_INTERRUPT_HANDLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PAGE_FAULT_INTERRUPT_HANDLER_HPP

#include <panic.h>
#include <cstring>
#include "interrupt_handler.hpp"

class PageFaultInterruptHandler : public InterruptHandler {
public:
    PageFaultInterruptHandler();

    void int_handler(const ParameterPackage& params) override;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_PAGE_FAULT_INTERRUPT_HANDLER_HPP
