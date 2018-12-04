#include "catch.hpp"
#include <platform/x86_64/physical_memory_map.hpp>

TEST_CASE("test_x86_64_physical_memory_map_default_ctor", "[x86_64]")
{
    x86_64::PhysicalMemoryMap phys_map;
}
