#ifndef FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP

#include <common/vector.hpp>
#include <common/unique_pointer.hpp>
#include <thread.hpp>
#include <interrupt_lock.hpp>

class Scheduler {
public:
    using ThreadPointer = UniquePointer<Thread, InterruptLock>;

    Scheduler() noexcept;

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
    InterruptLock lock_;
    Vector<ThreadPointer> runnable_;
    Vector<ThreadPointer> exhausted_;
    ThreadPointer current_thread_;

    bool can_yield() noexcept;
    void take_current_thread_from_runnable_queue() noexcept;
    void swap_runnable_and_exhausted_if_necessary() noexcept;
    void move_to_next_runnable_thread() noexcept;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP
