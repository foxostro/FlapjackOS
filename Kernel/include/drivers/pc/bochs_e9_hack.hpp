#ifndef FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_BOCHS_E9_HACK_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_BOCHS_E9_HACK_HPP

#include <common/text_output_stream.hpp>

// Logging via the Bochs "e9 hack" which allows port e9 to be used for output.
class BochsE9Hack : public TextOutputStream {
public:
    void putchar(char ch) override;
    void puts(const char *s) override;

private:
    static constexpr unsigned BOCHS_LOGGER_PORT = 0xe9;
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_BOCHS_E9_HACK_HPP
