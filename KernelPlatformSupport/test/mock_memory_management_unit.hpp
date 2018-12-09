#ifndef KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
#define KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP

#include <cstdint>

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

private:
    uintptr_t cr3_;
};

#endif // KERNELPLATFORMSUPPORT_TEST_MOCK_MEMORY_MANAGEMENT_UNIT_HPP
