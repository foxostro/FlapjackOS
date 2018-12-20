#include <platform/x86_64/interrupt_controller.hpp>
#include <platform/x86_64/panic_interrupt_handler.hpp>
#include <platform/x86_64/page_fault_interrupt_handler.hpp>
#include <simple_device_interrupt_handler.hpp>
#include <logger.hpp>

namespace x86_64 {

InterruptController::InterruptController()
 : interrupt_dispatcher_(hardware_interrupt_controller_),
   are_interrupts_ready_(false)
 {}

void InterruptController::initialize_hardware()
{
    hardware_interrupt_controller_.init();
}

void InterruptController::setup()
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

void InterruptController::install(unsigned irq, InterruptDispatcher::Handler handler)
{
    interrupt_dispatcher_.set_handler(irq, handler);
}

void InterruptController::become_ready()
{
    interrupt_dispatcher_.set_should_panic_on_null_handler(false);
    are_interrupts_ready_ = true;
    enable_interrupts();
}

void InterruptController::disable_interrupts()
{
    if (are_interrupts_ready_) {
        hardware_interrupt_controller_.disable_interrupts();
    }
}

void InterruptController::enable_interrupts()
{
    if (are_interrupts_ready_) {
        hardware_interrupt_controller_.enable_interrupts();
    }
}

void InterruptController::dispatch(void* params_raw)
{
    InterruptParameters* params = (InterruptParameters*)params_raw;
    assert(params);
    interrupt_dispatcher_.dispatch(*params);
}

} // namespace x86_64
