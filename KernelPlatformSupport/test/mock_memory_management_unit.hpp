#ifndef KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
#define KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>

// This class assuming that so-called "physical" addresses are the same size as
// linear addresses. That assumption is invalid when we try to build an x86_64
// test runner for i386 paging objects.
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

private:
    uintptr_t cr3_;
};

#endif // KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
