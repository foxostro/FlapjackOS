#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP

#include "context_switch.hpp"
#include <thread.hpp>
#include <cstdint>

namespace i386 {

class Thread_i386_Base : public ::Thread {
public:
    char* switch_to(Lock& lock) override
    {
        lock.unlock();
        char* old_stack_pointer = nullptr;
        i386_context_switch(&old_stack_pointer, get_stack_pointer());
        return old_stack_pointer;
    }

    void switch_away(Lock& lock, ::Thread& next) override
    {
        lock.unlock();
        i386_context_switch(&get_stack_pointer(), next.get_stack_pointer());
    }
};

// Represents a thread of execution on i386.
class Thread final : public Thread_i386_Base {
public:
    static constexpr uint32_t InitialRegisterValue = 0xcdcdcdcd;

    virtual ~Thread() = default;
    Thread() = default;
    Thread(Thread&& other) = default;
    Thread(const Thread&) = delete;

    explicit Thread(void (*function)())
    {
        stack_.push(/*EIP=*/reinterpret_cast<uintptr_t>(vanish));
        stack_.push(/*EIP=*/reinterpret_cast<uintptr_t>(function));
        char* EBP = stack_.stack_pointer - sizeof(EBP);
        stack_.push(/*EBP=*/EBP);
        char* ESP = stack_.stack_pointer;
        stack_.push(/*POPA, EAX=*/InitialRegisterValue);
        stack_.push(/*POPA, ECX=*/InitialRegisterValue);
        stack_.push(/*POPA, EDX=*/InitialRegisterValue);
        stack_.push(/*POPA, EBX=*/InitialRegisterValue);
        stack_.push(/*POPA, ESP=*/ESP);
        stack_.push(/*POPA, EBP=*/EBP);
        stack_.push(/*POPA, ESI=*/InitialRegisterValue);
        stack_.push(/*POPA, EDI=*/InitialRegisterValue);
    }

    char*& get_stack_pointer() override
    {
        return stack_.stack_pointer;
    }

private:
    StaticStack<PAGE_SIZE> stack_;
};

// The kernel init thread is special.
// The init thread is running even before this object or the scheduler is
// instantiated.
class InitThread final : public Thread_i386_Base {
public:
    virtual ~InitThread() = default;

    InitThread() : stack_pointer_(nullptr) {}
    InitThread(InitThread&& other) = default;
    InitThread(const InitThread&) = delete;

    char*& get_stack_pointer() override
    {
        return stack_pointer_;
    }

private:
    char* stack_pointer_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
