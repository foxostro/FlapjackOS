#include <interrupt_dispatcher.hpp>
#include <panic.h>
#include <platform/pc/pic.h> // TODO: The InterruptDispatcher class needs to be more platform-agnostic.
#include <kernel.hpp>
#include <cstring>
#include <cassert>

InterruptDispatcher::InterruptDispatcher()
 : should_panic_on_null_handler_(true)
{}

void InterruptDispatcher::set_handler(unsigned interrupt_number, const Handler& handler)
{
    assert(interrupt_number < IDT_MAX);
    lock_.lock();
    handlers_[interrupt_number] = handler;
    lock_.unlock();
}

auto InterruptDispatcher::get_handler(unsigned interrupt_number) -> Handler
{
    lock_.lock();
    Handler handler = handlers_[interrupt_number];
    lock_.unlock();
    return handler;
}

void InterruptDispatcher::dispatch(const Params& params) noexcept
{
    assert(params.size == sizeof(Params));
    unsigned interrupt_number = params.interrupt_number;
    assert(interrupt_number < IDT_MAX);
    bool spurious = clear_hardware_interrupt(interrupt_number);
    if (!spurious) {
        auto handler = get_handler(interrupt_number);
        if (handler) {
            handler->int_handler(params);
        } else if (should_panic_on_null_handler()) {
            panic_on_null_handler(interrupt_number);
        }
    }
}

bool InterruptDispatcher::clear_hardware_interrupt(unsigned interrupt_number) noexcept
{
    // TODO: This feels messy. If I implement my own std::function then I pass a closure to InterruptDispatcher which makes the call to clear the PIC.
    return g_kernel.get_hardware_interrupt_controller().clear(interrupt_number);
}

void InterruptDispatcher::panic_on_null_handler(unsigned interrupt_number) noexcept
{
    panic("No handler for interrupt \"%s\" (%u)\n",
          get_interrupt_name(interrupt_number),
          interrupt_number);
}

const char* InterruptDispatcher::get_interrupt_name(unsigned interrupt_number) noexcept
{
    switch (interrupt_number) {
        case IDT_KEY:   return "Keyboard";
        case IDT_TIMER: return "Timer";
        case IDT_DE:    return "Division Error";
        case IDT_DB:    return "Debug Exception";
        case IDT_NMI:   return "Non-Maskable Interrupt";
        case IDT_BP:    return "Breakpoint";
        case IDT_OF:    return "Overflow";
        case IDT_BR:    return "BOUND Range exceeded";
        case IDT_UD:    return "Undefined Opcode";
        case IDT_NM:    return "No Math coprocessor";
        case IDT_DF:    return "Double Fault.";
        case IDT_CSO:   return "Coprocessor Segment Overrun";
        case IDT_TS:    return "Invalid Task Segment Selector.";
        case IDT_NP:    return "Segment Not Present.";
        case IDT_SS:    return "Stack Segment Fault.";
        case IDT_GP:    return "General Protection Fault.";
        case IDT_PF:    return "Page Fault";
        case IDT_MF:    return "X87 Math Fault";
        case IDT_AC:    return "Alignment Check";
        case IDT_MC:    return "Machine Check";
        case IDT_XF:    return "SSE Floating Point Exception";
        default:        return "unknown";
    };
}

void InterruptDispatcher::set_should_panic_on_null_handler(bool should_panic)
{
    lock_.lock();
    should_panic_on_null_handler_ = should_panic;
    lock_.unlock();
}

bool InterruptDispatcher::should_panic_on_null_handler() noexcept
{
    bool should_panic; 
    lock_.lock();
    should_panic = should_panic_on_null_handler_;
    lock_.unlock();
    return should_panic;
}
