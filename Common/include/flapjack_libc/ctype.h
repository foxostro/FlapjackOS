#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H

#ifdef __cplusplus
extern "C" {
#else
#include <stdbool.h>
#endif

static inline bool isprint(int c)
{
    return (c >= ' ') && (c <= 126);
}

static inline int isdigit(int c)
{
    return (c >= '0') && (c <= '9');
}

#ifdef __cplusplus
} // extern "C"
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H
