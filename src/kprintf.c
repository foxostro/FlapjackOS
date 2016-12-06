#include <stdarg.h>
#include <stddef.h>

int kvsnprintf(char * restrict buf,
               size_t size,
               const char * restrict fmt,
               __attribute__((__unused__)) va_list args)
{
    int c = 0;

    if (buf == NULL || size == 0) {
        return 0;
    }

    for (char *str = buf; *fmt; fmt++)
    {
        *str++ = *fmt;
        c++;
    }

    return c;
}

int ksnprintf(char * restrict str,
              size_t size,
              const char * restrict fmt,
              ...)
{
    va_list args;
    va_start(args, fmt);
    int c = kvsnprintf(str, size, fmt, args);
    va_end(args);
    return c;
}
