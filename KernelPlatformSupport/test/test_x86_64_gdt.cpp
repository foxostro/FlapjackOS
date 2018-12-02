#include "catch.hpp"
#include <platform/x86_64/gdt.hpp>

// NOTE: As I'm sitting here thinking of how to unit test this, it occurs to me
// that there's really no point. The x86_64::GlobalDescriptorTable specifies
// a table of data with basically no logic. Tests for this could only repeat
// the expected data and assert that two are the same.

TEST_CASE("test_x86_64_gdt_default_ctor", "[x86_64]")
{
    x86_64::GlobalDescriptorTable table;
}