#include "catch.hpp"
#include <platform/x86_64/tss.hpp>

TEST_CASE("test_x86_64_tss_layout", "[x86_64]")
{
	// TaskStateSegment must match the layout specified in the Intel Manual,
	// volume 3a, section 7.7.
    REQUIRE(0x00 == offsetof(x86_64::TaskStateSegment, reserved0));
    REQUIRE(0x04 == offsetof(x86_64::TaskStateSegment, rsp0));
    REQUIRE(0x0C == offsetof(x86_64::TaskStateSegment, rsp1));
    REQUIRE(0x14 == offsetof(x86_64::TaskStateSegment, rsp2));
    REQUIRE(0x1C == offsetof(x86_64::TaskStateSegment, reserved1));
    REQUIRE(0x24 == offsetof(x86_64::TaskStateSegment, ist1));
    REQUIRE(0x2C == offsetof(x86_64::TaskStateSegment, ist2));
    REQUIRE(0x34 == offsetof(x86_64::TaskStateSegment, ist3));
    REQUIRE(0x3C == offsetof(x86_64::TaskStateSegment, ist4));
    REQUIRE(0x44 == offsetof(x86_64::TaskStateSegment, ist5));
    REQUIRE(0x4C == offsetof(x86_64::TaskStateSegment, ist6));
    REQUIRE(0x54 == offsetof(x86_64::TaskStateSegment, ist7));
    REQUIRE(0x5C == offsetof(x86_64::TaskStateSegment, reserved2));
    REQUIRE(0x64 == offsetof(x86_64::TaskStateSegment, reserved3));
    REQUIRE(0x66 == offsetof(x86_64::TaskStateSegment, io_map_base_address));
    REQUIRE(0x68 == sizeof(x86_64::TaskStateSegment));
}
