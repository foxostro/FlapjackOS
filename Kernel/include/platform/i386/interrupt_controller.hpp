#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_CONTROLLER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_CONTROLLER_HPP

#include "hardware_interrupt_controller.hpp"
#include "interrupt_parameters.hpp"
#include "interrupt_dispatcher.hpp"
#include "panic_interrupt_handler.hpp"
#include "page_fault_interrupt_handler.hpp"
#include <simple_device_interrupt_handler.hpp>
#include <cassert>
#include <logger.hpp>

namespace i386 {

template<typename Policy>
class GenericInterruptController {
public:
    using InterruptParameters = typename Policy::InterruptParameters;
    using HardwareInterruptController = typename Policy::HardwareInterruptController;
    using InterruptDispatcher = typename Policy::InterruptDispatcher;
    using PanicInterruptHandler = typename Policy::PanicInterruptHandler;
    using PageFaultInterruptHandler = typename Policy::PageFaultInterruptHandler;
    using Handler = typename InterruptDispatcher::Handler;

    GenericInterruptController()
     : interrupt_dispatcher_(hardware_interrupt_controller_),
       are_interrupts_ready_(false)
    {}

    // Initialize the underlying hardware interrupt controller.
    void initialize_hardware()
    {
        hardware_interrupt_controller_.init();
    }

    // Installs interrupt handlers for system standard interrupts and traps.
    void setup()
    {
        TRACE("begin");

        interrupt_dispatcher_.set_handler(IDT_DE,  new PanicInterruptHandler("Division Error", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_DB,  new PanicInterruptHandler("Debug Exception", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_NMI, new PanicInterruptHandler("Non-Maskable Interrupt", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_BP,  new PanicInterruptHandler("Breakpoint", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_OF,  new PanicInterruptHandler("Overflow", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_BR,  new PanicInterruptHandler("BOUND Range exceeded", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_UD,  new PanicInterruptHandler("Undefined Opcode", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_NM,  new PanicInterruptHandler("No Math coprocessor", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_DF,  new PanicInterruptHandler("Double Fault.", /* error_code_present = */ true));
        interrupt_dispatcher_.set_handler(IDT_CSO, new PanicInterruptHandler("Coprocessor Segment Overrun", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_TS,  new PanicInterruptHandler("Invalid Task Segment Selector.", /* error_code_present = */ true));
        interrupt_dispatcher_.set_handler(IDT_NP,  new PanicInterruptHandler("Segment Not Present.", /* error_code_present = */ true));
        interrupt_dispatcher_.set_handler(IDT_SS,  new PanicInterruptHandler("Stack Segment Fault.", /* error_code_present = */ true));
        interrupt_dispatcher_.set_handler(IDT_GP,  new PanicInterruptHandler("General Protection Fault.", /* error_code_present = */ true));
        interrupt_dispatcher_.set_handler(IDT_PF,  new PageFaultInterruptHandler());
        interrupt_dispatcher_.set_handler(IDT_MF,  new PanicInterruptHandler("X87 Math Fault", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_AC,  new PanicInterruptHandler("Alignment Check", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_MC,  new PanicInterruptHandler("Machine Check", /* error_code_present = */ false));
        interrupt_dispatcher_.set_handler(IDT_XF,  new PanicInterruptHandler("SSE Floating Point Exception", /* error_code_present = */ false));

        TRACE("end");
    }

    // Installs the specific handler for the given IRQ line.
    void install(unsigned irq, Handler handler)
    {
        interrupt_dispatcher_.set_handler(irq, handler);
    }

    // Become ready to receive hardware interrupt and then enable them.
    void become_ready()
    {
        interrupt_dispatcher_.set_should_panic_on_null_handler(false);
        are_interrupts_ready_ = true;
        enable_interrupts();
    }

    // Disables interrupts.
    // If the interrupt controller is not ready then this does nothing.
    void disable_interrupts()
    {
        if (are_interrupts_ready_) {
            hardware_interrupt_controller_.disable_interrupts();
        }
    }

    // Enables interrupts.
    // If the interrupt controller is not ready then this does nothing.
    void enable_interrupts()
    {
        if (are_interrupts_ready_) {
            hardware_interrupt_controller_.enable_interrupts();
        }
    }

    // Redirect the interrupt to the appropriate handler.
    void dispatch(void* params_raw)
    {
        InterruptParameters* params = (InterruptParameters*)params_raw;
        assert(params);
        interrupt_dispatcher_.dispatch(*params);
    }

private:
    HardwareInterruptController hardware_interrupt_controller_;
    InterruptDispatcher interrupt_dispatcher_;
    bool are_interrupts_ready_;
};

struct InterruptControllerPolicy {
    using InterruptParameters = i386::InterruptParameters;
    using HardwareInterruptController = i386::HardwareInterruptController;
    using InterruptDispatcher = i386::InterruptDispatcher;
    using PanicInterruptHandler = i386::PanicInterruptHandler;
    using PageFaultInterruptHandler = i386::PageFaultInterruptHandler;
};

using InterruptController = GenericInterruptController<InterruptControllerPolicy>;

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_INTERRUPT_CONTROLLER_HPP
