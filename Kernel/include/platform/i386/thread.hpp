#ifndef FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP

#include "context_switch.hpp"
#include <page_size.hpp>
#include <common/static_stack.hpp>
#include <cstdint>

namespace i386 {

// Represents a thread of execution on i386.
class Thread {
public:
    Thread() = default;
    
    explicit Thread(void* instruction_pointer)
    {
        stack_.push(/*EIP=*/reinterpret_cast<uintptr_t>(instruction_pointer));
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

    void context_switch()
    {
        char* old_stack_pointer = nullptr;
        i386_context_switch(&old_stack_pointer, stack_.stack_pointer);
    }

    void context_switch(Thread& next)
    {
        i386_context_switch(&stack_.stack_pointer, next.stack_.stack_pointer);
    }

private:
    static constexpr uint32_t InitialRegisterValue = 0xcdcdcdcd;
    StaticStack<PAGE_SIZE> stack_;
};

} // namespace i386

#endif // FLAPJACKOS_KERNEL_INCLUDE_PLATFORM_I386_THREAD_HPP
