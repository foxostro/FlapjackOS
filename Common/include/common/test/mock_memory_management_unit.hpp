#ifndef COMMON_INCLUDE_COMMON_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
#define COMMON_INCLUDE_COMMON_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>
#include <map>

// MMU for a 32-bit address space.
class MockMemoryManagementUnit32
{
public:
    using PhysicalAddress = uint32_t;
    using LinearAddress = uintptr_t;

    MockMemoryManagementUnit32() : cr3_(0) {}

    void set_cr3(PhysicalAddress value)
    {
        cr3_ = value;
    }

    PhysicalAddress get_cr3()
    {
        return cr3_;
    }

    void reload()
    {
        // nothing to do
    }

    LinearAddress convert_physical_to_logical_address(PhysicalAddress physical_address)
    {
        LinearAddress result = mapPhysicalToLinear_.find(physical_address)->second;
        return result;
    }
    
    PhysicalAddress convert_logical_to_physical_address(LinearAddress logical_address)
    {
        PhysicalAddress result = mapLinearToPhysical_.find(logical_address)->second;
        return result;
    }

    // Gets the address of the start of the kernel virtual memory region.
    // This is the start of the so-called higher-half.
    constexpr PhysicalAddress get_kernel_virtual_start_address()
    {
        return KERNEL_VIRTUAL_START_ADDR_32BIT;
    }

    void register_address(LinearAddress linear_address,
                          PhysicalAddress physical_address)
    {
        mapLinearToPhysical_[linear_address] = physical_address;
        mapPhysicalToLinear_[physical_address] = linear_address;
    }

private:
    PhysicalAddress cr3_;
    std::map<LinearAddress, PhysicalAddress> mapLinearToPhysical_;
    std::map<PhysicalAddress, LinearAddress> mapPhysicalToLinear_;
};

// MMU for a 64-bit address space.
class MockMemoryManagementUnit64
{
public:
    MockMemoryManagementUnit64() : cr3_(0) {}

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
    constexpr uint64_t get_kernel_virtual_start_address()
    {
        return KERNEL_VIRTUAL_START_ADDR_64BIT;
    }

private:
    uint64_t cr3_;
};

#endif // COMMON_INCLUDE_COMMON_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
