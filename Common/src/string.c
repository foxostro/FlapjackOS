#include "flapjack_libc/string.h"

size_t STRLEN(const char *s)
{
    size_t c = 0;
    if (s) for (; *s; ++c, ++s);
    return c;
}

size_t STRNLEN(const char *s, size_t maxlen)
{
    size_t c = 0;
    if (s) for (; c < maxlen && *s; ++c, ++s);
    return c;
}

void* MEMCPY(void * restrict dst, const void * restrict src, size_t n)
{
    // We could do better, but this works.

    for (size_t i = 0; i < n; ++i) {
        ((char *)dst)[i] = ((const char *)src)[i];
    }

    return dst;
}

void* MEMMOVE(void *dst, const void *src, size_t n)
{
    // We could do better, but this works.

    if (src != dst) {
        size_t begin, end;
        int step;

        if (dst > src) {
            begin = n - 1;
            end = ~0;
            step = -1;
        } else {
            begin = 0;
            end = n;
            step = 1;
        }

        for (size_t i = begin; i != end; i += step) {
            ((char *)dst)[i] = ((const char *)src)[i];
        }
    }

    return dst;
}

#if 0
int memcmp(const void *s1, const void *s2, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        if (s1[i] != s2[i]) {
            return s1[i] - s2[i];
        }
    }

    return 0;
}
#endif

void* MEMSET(void *dst, int value, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        ((char *)dst)[i] = (unsigned char)value;
    }

    return dst;
}
