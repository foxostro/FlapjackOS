#ifndef FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_SERIAL_HPP
#define FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_SERIAL_HPP

#include <cstddef>
#include <common/text_output_stream.hpp>

class Serial : public TextOutputStream {
public:
    Serial();
    void puts(const char* str) override;
    void putchar(char c) override;

private:
    static constexpr unsigned COM1 = 0x3F8;

    void pause();
};

#endif // FLAPJACKOS_KERNEL_INCLUDE_DRIVERS_PC_SERIAL_HPP
