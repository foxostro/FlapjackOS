#ifndef FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP

#include <page_size.hpp>
#include <interrupt_lock.hpp>
#include <common/static_stack.hpp>

class Thread {
public:
    virtual ~Thread() = default;
    virtual char* switch_to(InterruptLock& lock) = 0;
    virtual void switch_away(InterruptLock& lock, Thread& next) = 0;
};

// The current thread yields the remainder of its time slice to the next one.
extern "C" void yield();

// The current thread exits.
extern "C" __attribute__((noreturn)) void vanish();

#endif // FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
