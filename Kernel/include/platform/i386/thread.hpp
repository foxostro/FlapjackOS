#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP

#include "context_switch.hpp"
#include <thread.hpp>
#include <cstdint>

namespace i386 {

// Represents a thread of execution on i386.
class Thread final : public ::Thread {
public:
    static constexpr uint32_t InitialRegisterValue = 0xcdcdcdcd;

    Thread() = delete;
    Thread(const Thread&) = delete;
    Thread(Thread&& other) : ::Thread(std::move(other)) {}

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

    char* switch_to() override
    {
        char* old_stack_pointer = nullptr;
        i386_context_switch(&old_stack_pointer, stack_.stack_pointer);
        return old_stack_pointer;
    }

    void switch_away(::Thread& next) override
    {
        i386_context_switch(&stack_.stack_pointer, next.stack_.stack_pointer);
    }
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
