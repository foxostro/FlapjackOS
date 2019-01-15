#ifndef FLAPJACKOS_KERNEL_INCLUDE_LOGGER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_LOGGER_HPP

#include <common/text_output_stream.hpp>
#include <common/unique_pointer.hpp>
#include <cstddef>

// Debug logging in the kernel
class Logger {
public:
    void set_text_output_stream(UniquePointer<TextOutputStream> stream);
    void log(const char *function, const char *fmt, ...);

private:
    static constexpr size_t LOGGER_MESSAGE_SIZE = 1024;
    UniquePointer<TextOutputStream> stream_;
};

// There's a single global logger.
extern Logger g_logger;

// The global logger is typically invoked through the TRACE macro.
#define TRACE(...) do { g_logger.log(__FUNCTION__, __VA_ARGS__); } while(0)

#endif // FLAPJACKOS_KERNEL_INCLUDE_LOGGER_HPP
