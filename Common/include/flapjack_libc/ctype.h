#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H

static inline bool isprint(int c)
{
    return (c >= ' ') && (c <= 126);
}

static inline int isdigit(int c)
{
    return (c >= '0') && (c <= '9');
}

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_CTYPE_H
