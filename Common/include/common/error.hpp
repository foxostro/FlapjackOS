#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_ERROR_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_ERROR_HPP

struct Error {
    int error_code = 0;
    const char* error_domain = nullptr;
    const char* error_description = nullptr;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_ERROR_HPP
