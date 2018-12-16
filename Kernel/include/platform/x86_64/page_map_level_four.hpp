#ifndef FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_FOUR_HPP
#define FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_FOUR_HPP

#include <page_size.hpp>
#include "page_map_level_four_entry.hpp"

namespace x86_64 {

struct alignas(PAGE_SIZE) PageMapLevelFour {
public:
    static constexpr size_t COUNT = 512;
    PageMapLevelFourEntry entries[COUNT];
};
static_assert(4096 == sizeof(PageMapLevelFour),
              "The hardware PML4 object has a length of 4096 bytes.");

} // namespace x86_64

#endif // FLAPJACKOS_KERNEPLATFORMSUPPORT_INCLUDE_PLATFORM_X86_64_PAGE_MAP_LEVEL_FOUR_HPP
