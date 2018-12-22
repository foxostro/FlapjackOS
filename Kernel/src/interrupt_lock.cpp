#include <interrupt_lock.hpp>
#include <interrupt_asm.h>
#include <logger.hpp>
#include <atomic>

static std::atomic<int> g_interrupt_lock_count{0};

void InterruptLock::lock()
{
    if (g_interrupt_lock_count.fetch_add(1) == 0) {
        ::disable_interrupts();
    }
}

void InterruptLock::unlock()
{
    if (g_interrupt_lock_count.fetch_sub(1) == 1) {
        ::enable_interrupts();
    }
}
