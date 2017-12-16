#pragma once

#include <stdlib.h>

// Panic is defined in libKernel. Need to figure out how to resolve this circular dependency.
__attribute__((noreturn)) void panic(const char * restrict fmt, ...);

#ifdef TESTING
#define assert(expr) ((void)((expr) ? 0 : (abort(), 0)))
#else
#define assert(expr) ((void)((expr) ? 0 : (panic("%s:%u: failed assertion `%s'", __FILE__, __LINE__, # expr), 0)))
#endif
