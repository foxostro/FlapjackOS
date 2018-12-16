#ifndef KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
#define KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>

#ifdef __i386__
// 32-bit system
class MockMemoryManagementUnit
{
public:
    MockMemoryManagementUnit() : cr3_(0) {}

    void set_cr3(uint32_t value)
    {
        cr3_ = value;
    }

    uint32_t get_cr3()
    {
        return cr3_;
    }

    void reload()
    {
        // nothing to do
    }

    uintptr_t convert_physical_to_logical_address(uint32_t physical_address)
    {
	    return physical_address;
    }
    
    uint32_t convert_logical_to_physical_address(uintptr_t logical_address)
    {
	    return logical_address;
    }

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    constexpr uintptr_t get_kernel_virtual_start_address()
    {
        return KERNEL_VIRTUAL_START_ADDR_32BIT;
    }

private:
    uint32_t cr3_;
};
#else
// 64-bit system
class MockMemoryManagementUnit
{
public:
    MockMemoryManagementUnit() : cr3_(0) {}

    void set_cr3(uint64_t value)
    {
        cr3_ = value;
    }

    uint64_t get_cr3()
    {
        return cr3_;
    }

    void reload()
    {
        // nothing to do
    }

    uintptr_t convert_physical_to_logical_address(uint64_t physical_address)
    {
	    return physical_address;
    }
    
    uint64_t convert_logical_to_physical_address(uintptr_t logical_address)
    {
	    return logical_address;
    }

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    constexpr uintptr_t get_kernel_virtual_start_address()
    {
        return KERNEL_VIRTUAL_START_ADDR_64BIT;
    }

private:
    uint64_t cr3_;
};
static_assert(sizeof(uint64_t) >= sizeof(uintptr_t),
              "Make sure we can store a pointer in a uint64_t.");
#endif

#endif // KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
