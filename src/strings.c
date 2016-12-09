#include <stddef.h>

void bzero(char *s, size_t n)
{
    for(size_t i = 0; i < n; ++i) {
        s[i] = 0;
    }
}

size_t strnlen(const char *s, size_t maxlen)
{
    size_t c;
    for(c = 0; c < maxlen && *s; ++c, ++s);
    return c;
}
