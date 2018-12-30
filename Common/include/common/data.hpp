#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_DATA_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_DATA_HPP

#include <cstddef>
#include <common/text_terminal.hpp>

// Non-owning reference to a block of data bytes in memory.
struct Data {
    size_t length;
    unsigned char* bytes;

    void print(TextTerminal &terminal)
    {
        for (size_t i = 0; i < length; ++i) {
            terminal.printf(" %x", (unsigned)bytes[i]);
        }
        terminal.printf("\n");
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_DATA_HPP
