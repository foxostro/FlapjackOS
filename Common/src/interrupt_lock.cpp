#include <common/interrupt_lock.hpp>

std::atomic<int> InterruptLock::count_{0};

static void nop() {}
void (*InterruptLock::enable_interrupts)() = ::nop;
void (*InterruptLock::disable_interrupts)() = ::nop;

void InterruptLock::lock()
{
    if (count_.fetch_add(1) == 0) {
        disable_interrupts();
    }
}

void InterruptLock::unlock()
{
    if (count_.fetch_sub(1) == 1) {
        enable_interrupts();
    }
}
