#ifndef FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP

#include <common/vector.hpp>
#include <common/unique_pointer.hpp>
#include <common/interrupt_lock.hpp>
#include <thread.hpp>

namespace SchedulerDetail {

// Disables interrupts while the lock is held. Prevents reentrant execution.
class SchedulerLock {
public:
    SchedulerLock() : is_locked_(false) {}
    SchedulerLock(const SchedulerLock& other) = delete;
    SchedulerLock(SchedulerLock&& other) = delete;

    void lock();
    void unlock();

private:
    InterruptLock lock_;
    bool is_locked_;
};

// The scheduler implementation with no locking or synchronization at all.
// This wrapped by a monitor which takes care of that stuff.
class SchedulerUnlocked {
public:
    using ThreadPointer = UniquePointer<Thread, InterruptLock>;

    // Adds a new runnable thread.
    // The scheduler takes ownership of the pointer.
    void add(ThreadPointer thread) noexcept;

    // Begin scheduling.
    // init_thread -- Represents the kernel init thread.
    void begin(ThreadPointer init_thread) noexcept;

    // The current thread yields to the next runnable thread.
    void yield() noexcept;

    // The current thread exits.
    __attribute__((noreturn)) void vanish() noexcept;

private:
    Vector<ThreadPointer> runnable_;
    Vector<ThreadPointer> exhausted_;
    ThreadPointer current_thread_;

    bool can_yield() noexcept;
    void take_current_thread_from_runnable_queue() noexcept;
    void swap_runnable_and_exhausted_if_necessary() noexcept;
    void move_to_next_runnable_thread() noexcept;
};

// Wraps a SchedulerUnlocked, applying locking and synchronization to calls.
class SchedulerMonitor {
public:
    using ThreadPointer = SchedulerUnlocked::ThreadPointer;

    SchedulerMonitor() noexcept;

    // Adds a new runnable thread.
    // The scheduler takes ownership of the pointer.
    void add(ThreadPointer thread) noexcept;

    // Begin scheduling.
    // init_thread -- Represents the kernel init thread.
    void begin(ThreadPointer init_thread) noexcept;

    // The current thread yields to the next runnable thread.
    void yield() noexcept;

    // The current thread exits.
    __attribute__((noreturn)) void vanish() noexcept;

    // When a thread starts, we need to unlock the scheduler.
    void unlock_at_thread_start() noexcept;

private:
    SchedulerLock lock_;
    SchedulerUnlocked impl_;
};

} // namespace SchedulerDetail

using Scheduler = SchedulerDetail::SchedulerMonitor;

#endif // FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP
