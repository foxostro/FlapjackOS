#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP

#include "context_switch.hpp"
#include <thread.hpp>
#include <cstdint>

namespace x86_64 {

// Represents a thread of execution on x86_64.
class Thread final : public ::Thread  {
public:
    static constexpr uint64_t InitialRegisterValue = 0xcdcdcdcdcdcdcdcd;

    Thread() = delete;
    Thread(const Thread&) = delete;
    Thread(Thread&& other) : ::Thread(std::move(other)) {}

    explicit Thread(void (*function)())
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

    char* switch_to() override
    {
        char* old_stack_pointer = nullptr;
        x86_64_context_switch(&old_stack_pointer, stack_.stack_pointer);
        return old_stack_pointer;
    }

    void switch_away(::Thread& next) override
    {
        x86_64_context_switch(&stack_.stack_pointer, next.stack_.stack_pointer);
    }
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_X86_64_THREAD_HPP
