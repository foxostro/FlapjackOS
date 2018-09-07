#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_ASSERT_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_ASSERT_H

#ifdef __cplusplus
extern "C" {
#endif

#ifdef TESTING

#ifdef __cplusplus
#include <cstdlib>
#else
#include <stdlib.h>
#endif

#define assert(expr) ((void)((expr) ? 0 : (abort(), 0)))
#define MAGIC_BREAK asm volatile("xchg %bx, %bx")

#else // TESTING

// Panic is defined in libKernel. Need to figure out how to resolve this circular dependency.
__attribute__((noreturn)) void panic(const char *fmt, ...);

#define assert(expr) ((void)((expr) ? 0 : (panic("%s:%u: failed assertion `%s'", __FILE__, __LINE__, # expr), 0)))
#define MAGIC_BREAK

#endif // TESTING

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_ASSERT_H
