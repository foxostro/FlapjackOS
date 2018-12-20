#include "catch.hpp"
#include <platform/x86_64/idt.hpp>

TEST_CASE("x86_64::InterruptDescriptor::set_segment_selector -- set ones", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_segment_selector(0b1111111111111111);
    REQUIRE(descriptor.data == 0b0000000000000000000000000000000011111111111111110000000000000000);
}

TEST_CASE("x86_64::InterruptDescriptor::set_segment_selector -- set zeroes", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000000000000000011111111111111110000000000000000;
    descriptor.set_segment_selector(0);
    REQUIRE(descriptor.data == 0);
}

TEST_CASE("x86_64::InterruptDescriptor::get_segment_selector -- all ones", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000000000000000011111111111111110000000000000000;
    REQUIRE(descriptor.get_segment_selector() == 0b1111111111111111);
}

TEST_CASE("x86_64::InterruptDescriptor::set_bit", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_bit(2);
    REQUIRE(descriptor.data == 0b100);
}

TEST_CASE("x86_64::InterruptDescriptor::clear_bit", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b111;
    descriptor.clear_bit(1);
    REQUIRE(descriptor.data == 0b101);
}

TEST_CASE("x86_64::InterruptDescriptor::set_type(InterruptGate)", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_type(descriptor.InterruptGate);
    REQUIRE(descriptor.data == 0b0000000000000000000011100000000000000000000000000000000000000000);
}

TEST_CASE("x86_64::InterruptDescriptor::set_type(TrapGate)", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_type(descriptor.TrapGate);
    REQUIRE(descriptor.data == 0b0000000000000000000011110000000000000000000000000000000000000000);
}

TEST_CASE("x86_64::InterruptDescriptor::get_type() -> InterruptGate", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000011100000000000000000000000000000000000000000;
    REQUIRE(descriptor.get_type() == descriptor.InterruptGate);
}

TEST_CASE("x86_64::InterruptDescriptor::get_type() -> TrapGate", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000011110000000000000000000000000000000000000000;
    REQUIRE(descriptor.get_type() == descriptor.TrapGate);
}

TEST_CASE("x86_64::InterruptDescriptor::set_dpl -- set ones", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_dpl(3);
    REQUIRE(descriptor.data == 0b0000000000000000011000000000000000000000000000000000000000000000);
}

TEST_CASE("x86_64::InterruptDescriptor::set_dpl -- set zeroes", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000011000000000000000000000000000000000000000000000;
    descriptor.set_dpl(0);
    REQUIRE(descriptor.data == 0);
}

TEST_CASE("x86_64::InterruptDescriptor::get_dpl", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000011000000000000000000000000000000000000000000000;
    REQUIRE(descriptor.get_dpl() == 3);
}

TEST_CASE("x86_64::InterruptDescriptor::set_present(true)", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000000000000000000000000000000000000000000000000;
    descriptor.set_present(true);
    REQUIRE((descriptor.data & (1ull<<47)) != 0);
}

TEST_CASE("x86_64::InterruptDescriptor::set_present(false)", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 1ull<<47;
    descriptor.set_present(false);
    REQUIRE((descriptor.data & (1ull<<47)) == 0);
}

TEST_CASE("x86_64::InterruptDescriptor::get_present()", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 1ull<<47;
    REQUIRE(descriptor.get_present() == true);
}

TEST_CASE("x86_64::InterruptDescriptor::set_offset(0)", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = ~0ull;
    descriptor.data2 = ~0ull;
    descriptor.set_offset(0);
    REQUIRE(descriptor.data == 0x0000ffffffff0000);
    REQUIRE(descriptor.data2 == 0xffffffff00000000);
}

TEST_CASE("x86_64::InterruptDescriptor::set_offset(0xcafebabedeadbeef)", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.data2 = 0;
    descriptor.set_offset(0xcafebabedeadbeef);
    REQUIRE(descriptor.data == 0xdead00000000beef);
    REQUIRE(descriptor.data2 == 0x00000000cafebabe);
}

TEST_CASE("x86_64::InterruptDescriptor::get_offset()", "[x86_64]")
{
    x86_64::InterruptDescriptor descriptor;
    descriptor.data = 0xdead00000000beef;
    descriptor.data2 = 0x00000000cafebabe;
    REQUIRE(descriptor.get_offset() == 0xcafebabedeadbeef);
}
