#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GDT_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GDT_HPP

#include <cstdint>
#include <cstddef>
#include "tss.hpp"

namespace x86_64 {

using GlobalDescriptorTableEntry = unsigned long long;
constexpr size_t GDT_ENTRY_SIZE = 8;
static_assert(GDT_ENTRY_SIZE == sizeof(GlobalDescriptorTableEntry),
	          "The Intel docs specify that each entry in the GDT is 8 bytes.");

// Intel manual volume 3a, section 3.5.1, specifies that a GDT should be aligned 
// on an eight byte boundary.
constexpr size_t GDT_TABLE_ALIGNMENT = 8;

class GlobalDescriptorTable {
public:
    void init(TaskStateSegment *tss);
    void load();

private:
	static constexpr size_t NUMBER_OF_ENTRIES = 6;
    alignas(GDT_TABLE_ALIGNMENT) GlobalDescriptorTableEntry entries_[NUMBER_OF_ENTRIES];
};

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_GDT_HPP
