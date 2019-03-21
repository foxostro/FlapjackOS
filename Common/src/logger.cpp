#include <common/logger.hpp>
#include <cstdarg>
#include <cstdio>
#include <cassert>

Logger g_logger;

void Logger::set_text_output_stream(UniquePointer<TextOutputStream> stream)
{
    assert(stream);
    stream_ = std::move(stream);
}

void Logger::log(const char *function, const char *fmt, ...)
{
    assert(stream_);
    
    char message[LOGGER_MESSAGE_SIZE] = {0};
    char buffer[LOGGER_MESSAGE_SIZE] = {0};
    va_list args;

    va_start(args, fmt);
    vsnprintf(message, sizeof(message), fmt, args);
    va_end(args);

    snprintf(buffer, sizeof(buffer), "%s: %s\n", function, message);

    stream_->puts(buffer);
}
