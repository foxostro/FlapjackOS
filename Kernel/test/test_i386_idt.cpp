#include "catch.hpp"
#include <platform/i386/idt.hpp>

TEST_CASE("i386::InterruptDescriptor::set_segment_selector -- set ones", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_segment_selector(0b1111111111111111);
    REQUIRE(descriptor.data == 0b0000000000000000000000000000000011111111111111110000000000000000);
}

TEST_CASE("i386::InterruptDescriptor::set_segment_selector -- set zeroes", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000000000000000011111111111111110000000000000000;
    descriptor.set_segment_selector(0);
    REQUIRE(descriptor.data == 0);
}

TEST_CASE("i386::InterruptDescriptor::get_segment_selector -- all ones", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000000000000000011111111111111110000000000000000;
    REQUIRE(descriptor.get_segment_selector() == 0b1111111111111111);
}

TEST_CASE("i386::InterruptDescriptor::set_bit", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_bit(2);
    REQUIRE(descriptor.data == 0b100);
}

TEST_CASE("i386::InterruptDescriptor::clear_bit", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b111;
    descriptor.clear_bit(1);
    REQUIRE(descriptor.data == 0b101);
}

TEST_CASE("i386::InterruptDescriptor::set_type(TaskGate)", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_type(descriptor.TaskGate);
    REQUIRE(descriptor.data == 0x50000000000);
}

TEST_CASE("i386::InterruptDescriptor::set_type(InterruptGate)", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_type(descriptor.InterruptGate);
    REQUIRE(descriptor.data == 0b0000000000000000000011100000000000000000000000000000000000000000);
}

TEST_CASE("i386::InterruptDescriptor::set_type(TrapGate)", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_type(descriptor.TrapGate);
    REQUIRE(descriptor.data == 0b0000000000000000000011110000000000000000000000000000000000000000);
}

TEST_CASE("i386::InterruptDescriptor::get_type() -> TaskGate", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0x50000000000;
    REQUIRE(descriptor.get_type() == descriptor.TaskGate);
}

TEST_CASE("i386::InterruptDescriptor::get_type() -> InterruptGate", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000011100000000000000000000000000000000000000000;
    REQUIRE(descriptor.get_type() == descriptor.InterruptGate);
}

TEST_CASE("i386::InterruptDescriptor::get_type() -> TrapGate", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000011110000000000000000000000000000000000000000;
    REQUIRE(descriptor.get_type() == descriptor.TrapGate);
}

TEST_CASE("i386::InterruptDescriptor::set_dpl -- set ones", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_dpl(3);
    REQUIRE(descriptor.data == 0b0000000000000000011000000000000000000000000000000000000000000000);
}

TEST_CASE("i386::InterruptDescriptor::set_dpl -- set zeroes", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000011000000000000000000000000000000000000000000000;
    descriptor.set_dpl(0);
    REQUIRE(descriptor.data == 0);
}

TEST_CASE("i386::InterruptDescriptor::get_dpl", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000011000000000000000000000000000000000000000000000;
    REQUIRE(descriptor.get_dpl() == 3);
}

TEST_CASE("i386::InterruptDescriptor::set_present(true)", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0b0000000000000000000000000000000000000000000000000000000000000000;
    descriptor.set_present(true);
    REQUIRE((descriptor.data & (1ull<<47)) != 0);
}

TEST_CASE("i386::InterruptDescriptor::set_present(false)", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 1ull<<47;
    descriptor.set_present(false);
    REQUIRE((descriptor.data & (1ull<<47)) == 0);
}

TEST_CASE("i386::InterruptDescriptor::get_present()", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 1ull<<47;
    REQUIRE(descriptor.get_present() == true);
}

TEST_CASE("i386::InterruptDescriptor::set_offset(0)", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = ~0ull;
    descriptor.set_offset(0);
    REQUIRE(descriptor.data == 0x0000ffffffff0000);
}

TEST_CASE("i386::InterruptDescriptor::set_offset(0xdeadbeef)", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0;
    descriptor.set_offset(0xdeadbeef);
    REQUIRE(descriptor.data == 0xdead00000000beef);
}

TEST_CASE("i386::InterruptDescriptor::get_offset()", "[i386]")
{
    i386::InterruptDescriptor descriptor;
    descriptor.data = 0xdead00000000beef;
    REQUIRE(descriptor.get_offset() == 0xdeadbeef);
}
