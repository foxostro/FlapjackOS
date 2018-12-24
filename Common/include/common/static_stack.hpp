#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_STATIC_STACK_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_STATIC_STACK_HPP

#include <cstddef>
#include <cstring> // for memset, memcpy

// Statically allocated, unstructured stack.
template<size_t StackSize>
class StaticStack {
public:
    StaticStack()
    {
        memset(backing_, 0, sizeof(backing_));
        stack_pointer = backing_ + sizeof(backing_);
    }

    StaticStack(const StaticStack& other)
    {
        memmove(backing_, other.backing_, sizeof(backing_));
        ptrdiff_t offset = other.stack_pointer - other.backing_;
        stack_pointer = backing_ + offset;
    }

    StaticStack(StaticStack&& other)
    {
        memmove(backing_, other.backing_, sizeof(backing_));
        ptrdiff_t offset = other.stack_pointer - other.backing_;
        stack_pointer = backing_ + offset;
    }

    template<typename T>
    void push(T value)
    {
        stack_pointer -= sizeof(value);
        memcpy(stack_pointer, &value, sizeof(value));
    }

    char* stack_pointer;

private:
    alignas(alignof(void*)) char backing_[StackSize];
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_STATIC_STACK_HPP
