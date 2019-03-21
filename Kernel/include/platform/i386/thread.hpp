#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP

#include "context_switch.hpp"
#include <thread.hpp>
#include <page_size.hpp>
#include <common/static_stack.hpp>
#include <cstdint>

namespace i386 {

class Thread_i386_Base : public Thread {
public:
    void switch_away(Thread& next) override
    {
        i386_context_switch(&get_stack_pointer(), next.get_stack_pointer());
    }
};

// Represents a thread of execution on i386.
class Thread_i386 final : public Thread_i386_Base {
public:
    static constexpr uint32_t InitialRegisterValue = 0xcdcdcdcd;

    virtual ~Thread_i386() = default;
    Thread_i386() = delete;
    Thread_i386(Thread_i386&& other) = default;
    Thread_i386(const Thread_i386&) = delete;

    explicit Thread_i386(void (*function)(void*), void* param)
    {
        stack_.push4(/*second parameter=*/reinterpret_cast<uint32_t>(function));
        stack_.push4(/*first parameter=*/reinterpret_cast<uint32_t>(param));
        stack_.push4(/*return address=*/reinterpret_cast<uint32_t>(vanish));
        stack_.push4(/*EIP=*/reinterpret_cast<uintptr_t>(thread_start));
        char* EBP = stack_.stack_pointer - sizeof(EBP);
        stack_.push(/*EBP=*/reinterpret_cast<uint32_t>(EBP));
        char* ESP = stack_.stack_pointer;
        stack_.push4(/*POPA, EAX=*/InitialRegisterValue);
        stack_.push4(/*POPA, ECX=*/InitialRegisterValue);
        stack_.push4(/*POPA, EDX=*/InitialRegisterValue);
        stack_.push4(/*POPA, EBX=*/InitialRegisterValue);
        stack_.push4(/*POPA, ESP=*/reinterpret_cast<uint32_t>(ESP));
        stack_.push4(/*POPA, EBP=*/reinterpret_cast<uint32_t>(EBP));
        stack_.push4(/*POPA, ESI=*/InitialRegisterValue);
        stack_.push4(/*POPA, EDI=*/InitialRegisterValue);
        
        // We can't simply point fxrstor at a bunch of zeroes. So use real data.
        // This sets the initial FPU/SSE state when we enter thread_start(),
        // which will immediately be blown away by an invocation of fninit.
        constexpr size_t FXSAVE_DATA_SIZE = 512;
        constexpr size_t FXSAVE_DATA_ALIGN = 16;
        stack_.stack_pointer -= FXSAVE_DATA_SIZE;
        assert(reinterpret_cast<uintptr_t>(stack_.stack_pointer) % FXSAVE_DATA_ALIGN == 0);
        asm volatile("fxsave (%0)" :: "a"(stack_.stack_pointer));
    }

    char*& get_stack_pointer() override
    {
        return stack_.stack_pointer;
    }

private:
    StaticStack<PAGE_SIZE> stack_;
};

// An i386 thread which does not own the memory for its own stack.
// This is useful to represent the initial kernel thread in the scheduler as
// that thread stack is allocated at boot time before the memory subsystem is
// even ready.
class Thread_i386_ExternalStack final : public Thread_i386_Base {
public:
    virtual ~Thread_i386_ExternalStack() = default;

    Thread_i386_ExternalStack() : stack_pointer_(nullptr) {}
    Thread_i386_ExternalStack(Thread_i386_ExternalStack&& other) = default;
    Thread_i386_ExternalStack(const Thread_i386_ExternalStack&) = delete;

    char*& get_stack_pointer() override
    {
        return stack_pointer_;
    }

private:
    char* stack_pointer_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
