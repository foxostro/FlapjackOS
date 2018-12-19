#include <platform/x86_64/interrupt_controller.hpp>
#include <cassert>

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
    assert(!"stub");
}

void InterruptController::install(unsigned irq, InterruptDispatcher::Handler handler)
{
    (void)irq;
    (void)handler;
    assert(!"stub");
}

void InterruptController::become_ready()
{
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
