#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_STATIC_STACK_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_STATIC_STACK_HPP

#include <cstddef>
#include <cstring> // for memset, memcpy

// Statically allocated, unstructured stack.
template<size_t StackSize>
class StaticStack {
public:
    // Default ctor
    StaticStack()
    {
        memset(backing_, 0, sizeof(backing_));
        stack_pointer = backing_ + sizeof(backing_);
    }

    // Copy ctor
    StaticStack(const StaticStack& other)
    {
        memmove(backing_, other.backing_, sizeof(backing_));
        ptrdiff_t offset = other.stack_pointer - other.backing_;
        stack_pointer = backing_ + offset;
    }

    // Move ctor
    StaticStack(StaticStack&& other)
    {
        memmove(backing_, other.backing_, sizeof(backing_));
        ptrdiff_t offset = other.stack_pointer - other.backing_;
        stack_pointer = backing_ + offset;
    }

    // Copy-assignment operator
    StaticStack& operator=(const StaticStack &other)
    {
        if (this != &other) {
            memmove(backing_, other.backing_, sizeof(backing_));
            ptrdiff_t offset = other.stack_pointer - other.backing_;
            stack_pointer = backing_ + offset;
        }
        return *this;
    }

    // Move-assignment operator
    StaticStack& operator=(StaticStack &&other)
    {
        if (this != &other) {
            memmove(backing_, other.backing_, sizeof(backing_));
            ptrdiff_t offset = other.stack_pointer - other.backing_;
            stack_pointer = backing_ + offset;
        }
        return *this;
    }

    void push4(uint32_t value)
    {
        stack_pointer -= sizeof(value);
        memcpy(stack_pointer, &value, sizeof(value));
    }

    void push8(uint64_t value)
    {
        stack_pointer -= sizeof(value);
        memcpy(stack_pointer, &value, sizeof(value));
    }

    template<typename T>
    void push(T value)
    {
        stack_pointer -= sizeof(value);
        memcpy(stack_pointer, &value, sizeof(value));
    }

    void push(size_t size, const char *bytes)
    {
        assert(bytes);
        stack_pointer -= size;
        memcpy(stack_pointer, bytes, size);
    }
    
    template<typename T>
    T pop()
    {
        T result;
        memcpy(&result, stack_pointer, sizeof(result));
        stack_pointer += sizeof(result);
        return result;
    }
    
    template<typename T>
    T peek(int delta_objects)
    {
        T result;
        memcpy(&result, stack_pointer + sizeof(result)*delta_objects, sizeof(result));
        return result;
    }

    char* stack_pointer;

private:
    // Let's align thread stacks on 16 byte boundaries so they will work fine
    // with SSE in the future.
    static constexpr size_t ALIGNMENT = 16;

    alignas(ALIGNMENT) char backing_[StackSize];
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_STATIC_STACK_HPP
