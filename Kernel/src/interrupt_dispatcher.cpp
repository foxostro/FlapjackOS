#include <interrupt_dispatcher.hpp>
#include <panic.h>
#include <pic.h>
#include <interrupt_asm.h>
#include <cstring>
#include <cassert>

InterruptDispatcher::InterruptDispatcher() = default;

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

void InterruptDispatcher::dispatch(unsigned interrupt_number, const Params& params) noexcept
{
    assert(interrupt_number < IDT_MAX);
    bool spurious = pic_clear(interrupt_number);
    if (!spurious) {
        auto handler = get_handler(interrupt_number);
        if (handler) {
            handler->int_handler(params);
        }
    }
}
