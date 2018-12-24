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

    // Yield to the next runnable thread.
    void yield();

    template<typename T>
    void exchange(T& a, T& b)
    {
        auto temp = std::move(a);
        a = std::move(b);
        b = std::move(a);
    }

private:
    Vector<Thread*> runnable_; // AFOX_TODO: might want these to use a smart pointer class
    Vector<Thread*> exhausted_;
    Thread* current_thread_;
    // AFOX_TODO: need to add some lock to Scheduler. Maybe InterruptLock?
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_SCHEDULER_HPP
