#define CATCH_CONFIG_MAIN
#include "catch.hpp"

void lgdt(__attribute__((unused)) void *gdt, __attribute__((unused)) unsigned limit)
{
    // satisfy the linker
}
