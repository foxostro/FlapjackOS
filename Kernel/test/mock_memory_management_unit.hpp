#ifndef FLAPJACKOS_KERNEL_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
#define FLAPJACKOS_KERNEL_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>
#include <map>
#include <boot.h> // for KERNEL_VIRTUAL_START_ADDR_{32,64}BIT
#include <hardware_memory_management_unit.hpp>

// Mock MMU object.
// The host system address space is the same size as the mock address space.
class MockMemoryManagementUnit final : public HardwareMemoryManagementUnit
{
public:
    MockMemoryManagementUnit() : cr3_(0) {}

    void set_cr3(uintptr_t value)
    {
        cr3_ = value;
    }

    uintptr_t get_cr3() const override
    {
        return cr3_;
    }

    void reload() override
    {
        // nothing to do
    }

    uintptr_t convert_physical_to_logical_address(uintptr_t physical_address) const override
    {
        return physical_address;
    }
    
    uintptr_t convert_logical_to_physical_address(uintptr_t logical_address) const override
    {
        return logical_address;
    }

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    uintptr_t get_kernel_virtual_start_address() const override
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

#endif // FLAPJACKOS_KERNEL_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
