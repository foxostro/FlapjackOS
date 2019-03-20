#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP

#include "context_switch.hpp"
#include <thread.hpp>
#include <page_size.hpp>
#include <common/static_stack.hpp>
#include <cstdint>

namespace x86_64 {

class Thread_x86_64_Base : public Thread {
public:
    void switch_away(Thread& next) override
    {
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

    explicit Thread_x86_64(void (*function)(void*), uintptr_t param)
    {
        stack_.push8(/*RIP=*/reinterpret_cast<uint64_t>(vanish));
        stack_.push8(/*RIP=*/reinterpret_cast<uint64_t>(thread_start));
        char* RBP = stack_.stack_pointer - sizeof(RBP);
        stack_.push8(/*RBP=*/reinterpret_cast<uint64_t>(RBP));
        char* RSP = stack_.stack_pointer;
        stack_.push8(/*RAX=*/InitialRegisterValue);
        stack_.push8(/*RCX=*/InitialRegisterValue);
        stack_.push8(/*RDX=*/InitialRegisterValue);
        stack_.push8(/*RBX=*/InitialRegisterValue);
        stack_.push8(/*RSP=*/reinterpret_cast<uint64_t>(RSP));
        stack_.push8(/*RBP=*/reinterpret_cast<uint64_t>(RBP));
        stack_.push8(/*RSI=*/reinterpret_cast<uint64_t>(function));
        stack_.push8(/*RDI=*/static_cast<uint64_t>(param));
        stack_.push8(/*R8=*/InitialRegisterValue);
        stack_.push8(/*R9=*/InitialRegisterValue);
        stack_.push8(/*R10=*/InitialRegisterValue);
        stack_.push8(/*R11=*/InitialRegisterValue);
        stack_.push8(/*R12=*/InitialRegisterValue);
        stack_.push8(/*R13=*/InitialRegisterValue);
        stack_.push8(/*R14=*/InitialRegisterValue);
        stack_.push8(/*R15=*/InitialRegisterValue);
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
