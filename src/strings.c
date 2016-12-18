#include <stddef.h>

void bzero(char *s, size_t n)
{
    for (size_t i = 0; i < n; ++i) {
        s[i] = 0;
    }
}

size_t strlen(const char *s)
{
    size_t c;
    for (c = 0; *s; ++c, ++s);
    return c;
}

size_t strnlen(const char *s, size_t maxlen)
{
    size_t c;
    for (c = 0; c < maxlen && *s; ++c, ++s);
    return c;
}

void* memcpy(char * restrict const dst, const char * restrict const src, size_t n)
{
    // We could do better, but this works.
    for (size_t i = 0; i < n; ++i) {
        dst[i] = src[i];
    }

    return dst;
}
