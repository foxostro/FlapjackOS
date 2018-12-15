#include "catch.hpp"
#include <platform/i386/physical_memory_map.hpp>

TEST_CASE("test_i386_physical_memory_map_default_ctor", "[i386]")
{
    i386::PhysicalMemoryMap phys_map;
}
