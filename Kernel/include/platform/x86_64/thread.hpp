#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP

#include "context_switch.hpp"
#include <thread.hpp>
#include <cstdint>

namespace x86_64 {

class Thread_x86_64_Base : public Thread {
public:
    void switch_away(Lock& lock, Thread& next) override
    {
        lock.unlock();
        x86_64_context_switch(&get_stack_pointer(), next.get_stack_pointer());
    }
};

// Represents a thread of execution on x86_64.
class Thread_x86_64 final : public Thread_x86_64_Base {
public:
    static constexpr uint64_t InitialRegisterValue = 0xcdcdcdcdcdcdcdcd;

    virtual ~Thread_x86_64() = default;
    Thread_x86_64() = default;
    Thread_x86_64(Thread_x86_64&& other) = default;
    Thread_x86_64(const Thread_x86_64&) = delete;

    explicit Thread_x86_64(void (*function)())
    {
        stack_.push(/*RIP=*/reinterpret_cast<uintptr_t>(vanish));
        stack_.push(/*RIP=*/reinterpret_cast<uintptr_t>(function));
        char* RBP = stack_.stack_pointer - sizeof(RBP);
        stack_.push(/*RBP=*/RBP);
        char* RSP = stack_.stack_pointer;
        stack_.push(/*RAX=*/InitialRegisterValue);
        stack_.push(/*RCX=*/InitialRegisterValue);
        stack_.push(/*RDX=*/InitialRegisterValue);
        stack_.push(/*RBX=*/InitialRegisterValue);
        stack_.push(/*RSP=*/RSP);
        stack_.push(/*RBP=*/RBP);
        stack_.push(/*RSI=*/InitialRegisterValue);
        stack_.push(/*RDI=*/InitialRegisterValue);
        stack_.push(/*R8=*/InitialRegisterValue);
        stack_.push(/*R9=*/InitialRegisterValue);
        stack_.push(/*R10=*/InitialRegisterValue);
        stack_.push(/*R11=*/InitialRegisterValue);
        stack_.push(/*R12=*/InitialRegisterValue);
        stack_.push(/*R13=*/InitialRegisterValue);
        stack_.push(/*R14=*/InitialRegisterValue);
        stack_.push(/*R15=*/InitialRegisterValue);
    }

    char*& get_stack_pointer() override
    {
        return stack_.stack_pointer;
    }

private:
    StaticStack<PAGE_SIZE> stack_;
};

// An x86_64 thread which does not own the memory for its own stack.
// This is useful to represent the initial kernel thread in the scheduler as
// that thread stack is allocated at boot time before the memory subsystem is
// even ready.
class Thread_x86_64_ExternalStack final : public Thread_x86_64_Base {
public:
    virtual ~Thread_x86_64_ExternalStack() = default;

    Thread_x86_64_ExternalStack() : stack_pointer_(nullptr) {}
    Thread_x86_64_ExternalStack(Thread_x86_64_ExternalStack&& other) = default;
    Thread_x86_64_ExternalStack(const Thread_x86_64_ExternalStack&) = delete;

    char*& get_stack_pointer() override
    {
        return stack_pointer_;
    }

private:
    char* stack_pointer_;
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP
