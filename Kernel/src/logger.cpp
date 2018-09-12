#include <logger.hpp>
#include <inout.h>
#include <cstdarg>
#include <cstdio>

Logger g_logger;

constexpr unsigned Logger::BOCHS_LOGGER_PORT;

Logger::~Logger() = default;

Logger::Logger() = default;

void Logger::putchar(char ch)
{
    outb(BOCHS_LOGGER_PORT, ch);
}

void Logger::puts(const char *s)
{
    while (*s) {
        putchar(*s++);
    }
    putchar('\n');
}

void Logger::puts_no_newline(const char *s)
{
    while (*s) {
        putchar(*s++);
    }
}

void Logger::log(const char *function, const char *fmt, ...)
{
    char message[128] = {0};
    char buffer[256] = {0};
    va_list args;

    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    snprintf(buffer, sizeof(buffer), "%s: %s\n", function, message);
    puts_no_newline(buffer);
}
