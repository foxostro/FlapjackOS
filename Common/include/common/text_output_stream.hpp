#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_OUTPUT_STREAM_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_OUTPUT_STREAM_HPP

class TextOutputStream {
public:
    virtual ~TextOutputStream() = default;
    virtual void putchar(char ch) = 0;
    virtual void puts(const char* str) = 0;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_TEXT_OUTPUT_STREAM_HPP
