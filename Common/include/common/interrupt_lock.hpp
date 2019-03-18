#ifndef FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_LOCK_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_LOCK_HPP

#include <atomic>

// Mutual exclusion by enabling and disabling interrupts.
//
// This is only appropriate on a uniprocessor system.
//
// A SpinLock could never proceed if an interrupt occurs during a critical
// section on a uniprocessor system. To prevent this from occuring, the
// InterruptLock will enable and disable interrupts entirely during the critical
// section.
class InterruptLock {
public:
    InterruptLock() = default;
    InterruptLock(const InterruptLock& other) = delete;
    InterruptLock(InterruptLock&& other) = delete;

    void lock();
    void unlock();

    static void (*enable_interrupts)();
    static void (*disable_interrupts)();

private:
    static std::atomic<int> count_;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_INTERRUPT_LOCK_HPP
