#ifndef FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP

// Represents a single thread of execution on the CPU.
// The implementation must necesarily be platform-specific.
class Thread {
public:
    virtual ~Thread() = default;
    virtual void switch_away(Thread& next) = 0;
    virtual char*& get_stack_pointer() = 0;
};

// The current thread yields the remainder of its time slice to the next one.
extern "C" void yield() noexcept;

// The current thread exits.
extern "C" __attribute__((noreturn)) void vanish() noexcept;

// Entry point for new threads.
extern "C" void thread_start(void* param, void(*function)(void*)) noexcept;

#endif // FLAPJACKOS_KERNEL_INCLUDE_THREAD_HPP
