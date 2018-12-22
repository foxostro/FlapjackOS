#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_CONTROLLER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include "interrupt_dispatcher.hpp"
#include "panic_interrupt_handler.hpp"
#include "page_fault_interrupt_handler.hpp"
#include <platform/i386/interrupt_controller.hpp>

namespace x86_64 {

struct InterruptControllerPolicy {
    using InterruptParameters = x86_64::InterruptParameters;
    using HardwareInterruptController = x86_64::HardwareInterruptController;
    using InterruptDispatcher = x86_64::InterruptDispatcher;
    using PanicInterruptHandler = x86_64::PanicInterruptHandler;
    using PageFaultInterruptHandler = x86_64::PageFaultInterruptHandler;
};

using InterruptController = i386::GenericInterruptController<InterruptControllerPolicy>;

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_INTERRUPT_CONTROLLER_HPP
