#define CATCH_CONFIG_MAIN
#include "catch.hpp"

extern "C" uintptr_t get_cr3() { return 0; }
