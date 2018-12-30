#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_DATA_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_DATA_HPP

#include <cstddef>

// Non-owning reference to a block of data bytes in memory.
struct Data {
    size_t length;
    unsigned char* bytes;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_DATA_HPP
