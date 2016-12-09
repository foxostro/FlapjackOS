#include <stddef.h>

void bzero(char *s, size_t n)
{
    while(n)
    {
        *s++ = 0;
        n--;
    }
}

size_t strnlen(const char *s, size_t maxlen)
{
    size_t c;
    for(c = 0; c < maxlen && *s; ++c, ++s);
    return c;
}
