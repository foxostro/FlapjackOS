#ifndef FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP

#include <common/vector.hpp>
#include <thread.hpp>

class Scheduler {
public:
    Scheduler();

    // Adds a new runnable thread.
    // The scheduler takes ownership of the pointer.
    void add(Thread* thread);

    // Begin executing the first thread.
    void begin();

    // The current thread yields to the next runnable thread.
    void yield();

    // The current thread exits.
    __attribute__((noreturn)) void vanish();

    template<typename T>
    void exchange(T& a, T& b)
    {
        auto temp = std::move(a);
        a = std::move(b);
        b = std::move(temp);
    }

private:
    Vector<Thread*> runnable_; // AFOX_TODO: might want these to use a smart pointer class
    Vector<Thread*> exhausted_;
    Thread* current_thread_;
    // AFOX_TODO: need to add some lock to Scheduler. Maybe InterruptLock?

    void take_current_thread_from_runnable_queue();
    void swap_runnable_and_exhausted_if_necessary();
    void move_to_next_runnable_thread();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP
