#ifndef FLAPJACKOS_KERNEL_INCLUDE_LOGGER_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_LOGGER_HPP

// Debug logging in the kernel
class Logger {
public:
    ~Logger();
    Logger();
    void putchar(char ch);
    void puts(const char *s);
    void puts_no_newline(const char *s);
    void log(const char *function, const char *fmt, ...);

private:
    static constexpr unsigned BOCHS_LOGGER_PORT = 0xe9;
};

// There's a single global logger.
extern Logger g_logger;

// The global logger is typically invoked through the TRACE macro.
#define TRACE(...) do { g_logger.log(__FUNCTION__, __VA_ARGS__); } while(0)

#endif // FLAPJACKOS_KERNEL_INCLUDE_LOGGER_HPP
