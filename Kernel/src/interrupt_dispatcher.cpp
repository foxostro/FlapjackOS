#include <interrupt_dispatcher.hpp>
#include <panic.h>
#include <pic.h>
#include <interrupt_asm.h>
#include <cstring>
#include <cassert>

InterruptDispatcher::InterruptDispatcher()
{
    memset(handlers_, 0, sizeof(handlers_));
}

void InterruptDispatcher::set_handler(unsigned interrupt_number,
                                      InterruptHandler* handler)
{
    assert(interrupt_number < IDT_MAX);
    handlers_[interrupt_number] = handler;
}

void InterruptDispatcher::dispatch(unsigned interrupt_number, const Params& params) noexcept
{
    assert(interrupt_number < IDT_MAX);
    bool spurious = pic_clear(interrupt_number);
    if (!spurious) {
        InterruptHandler* handler = handlers_[interrupt_number];
        if (handler) {
            handler->int_handler(params);
        }
    }
}
