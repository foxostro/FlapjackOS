#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H

#ifdef TESTING
#error "Don't include this file while under test."
#endif

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

#include "ctype.inc"

#ifdef __cplusplus
} // extern "C"
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H
