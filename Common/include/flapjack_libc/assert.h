#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TESTING

#include <stdlib.h>
#define assert(expr) ((void)((expr) ? 0 : (abort(), 0)))

#else // TESTING

// Panic is defined in libKernel. Need to figure out how to resolve this circular dependency.
__attribute__((noreturn)) void panic(const char *fmt, ...);

#define assert(expr) ((void)((expr) ? 0 : (panic("%s:%u: failed assertion `%s'", __FILE__, __LINE__, # expr), 0)))

#endif // TESTING

#ifdef __cplusplus
}
#endif
