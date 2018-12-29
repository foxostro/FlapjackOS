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

    explicit Thread_i386(void (*function)(unsigned), unsigned param)
    {
        stack_.push(/*second parameter=*/reinterpret_cast<uintptr_t>(function));
        stack_.push(/*first parameter=*/static_cast<uint32_t>(param));
        stack_.push(/*return address=*/reinterpret_cast<uintptr_t>(vanish));
        stack_.push(/*EIP=*/reinterpret_cast<uintptr_t>(thread_start));
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
