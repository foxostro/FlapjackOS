#ifndef COMMON_INCLUDE_COMMON_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
#define COMMON_INCLUDE_COMMON_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>
#include <map>

// Mock MMU object.
// The host system address space is the same size as the mock address space.
class MockMemoryManagementUnit
{
public:
    MockMemoryManagementUnit() : cr3_(0) {}

    void set_cr3(uintptr_t value)
    {
        cr3_ = value;
    }

    uintptr_t get_cr3()
    {
        return cr3_;
    }

    void reload()
    {
        // nothing to do
    }

    uintptr_t convert_physical_to_logical_address(uintptr_t physical_address)
    {
        return physical_address;
    }
    
    uintptr_t convert_logical_to_physical_address(uintptr_t logical_address)
    {
        return logical_address;
    }

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    constexpr uintptr_t get_kernel_virtual_start_address()
    {
#if defined(__i386__)
        return KERNEL_VIRTUAL_START_ADDR_32BIT;
#elif defined(__x86_64__)
        return KERNEL_VIRTUAL_START_ADDR_64BIT;
#endif
    }

private:
    uintptr_t cr3_;
};

#endif // COMMON_INCLUDE_COMMON_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
