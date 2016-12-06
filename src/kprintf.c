#include <stdarg.h>
#include <stdbool.h>
#include <stddef.h>

static size_t insert_int(__attribute__((__unused__)) char **str,
                         __attribute__((__unused__)) size_t *size)
{
    return 0;
}

static size_t insert_char(char c, char **str, size_t *size)
{
    if (*size > 1) {
        *(*str)++ = c;
        (*size)--;
    }
    return 1;
}

size_t kvsnprintf(char * restrict buf,
                  size_t size,
                  const char * restrict fmt,
                  __attribute__((__unused__)) va_list args)
{
    size_t i = 0;
    char *str = buf;
    bool handlingPercent = false;

    if (buf == NULL || size == 0 || fmt == NULL) {
        return 0;
    }

    while (true)
    {
        char c = *fmt++;

        if (c == '\0') {
            if (size == 1) {
                *str = '\0';
                size = 0;
            }
            break;
        } else if (!handlingPercent) {
            if (c == '%') {
                handlingPercent = true;
            } else {
                i += insert_char(c, &str, &size);
            }
        } else {
            switch(c) {
                case 'd':
                    i += insert_int(&str, &size);
                    break;

                case 'c':
                    i += insert_char((char)va_arg(args, int), &str, &size);
                    break;

                default:
                    i += insert_char(c, &str, &size);
                    break;
            }

            handlingPercent = false;
        }
    }

    return i;
}

size_t ksnprintf(char * restrict str,
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
