#ifndef FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_TO_LONG_CONVERTER_H
#define FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_TO_LONG_CONVERTER_H

#include <cstddef>

class StringToLongConverter {
public:
    StringToLongConverter(const char* str, char** endptr, int base)
     : original_string_(str), str_(str), out_endptr_(endptr), base_(base)
    {}

    long convert()
    {
        if (!str_) {
            return 0; // `str_' must not be NULL.
        }

        if (base_ < 0 || base_ >= 35) {
            return 0; // `base_' must be in [0,35]
        }

        long multiplier = eat_sign();

        eat_base_prefix();

        const char* endptr = scan_to_end_of_number(str_);

        long result = 0;

        if (endptr == str_) {
            maybe_output_endptr(original_string_);
        } else {
            long unsigned_result = parse_number_digits(endptr);
            maybe_output_endptr(endptr);
            result = unsigned_result * multiplier;
        }

        return result;
    }

private:
    const char* original_string_;
    const char* str_;
    char** out_endptr_;
    int base_;

    void maybe_output_endptr(const char* endptr)
    {
        if (out_endptr_) {
            *out_endptr_ = const_cast<char*>(endptr);
        }
    }

    long eat_sign()
    {
        long multiplier = 1;

        if (*str_ == '+') {
            multiplier = 1;
            str_++;
        } else if (*str_ == '-') {
            multiplier = -1;
            str_++;
        }

        return multiplier;
    }

    void eat_base_prefix()
    {
        if ((base_ == 0 || base_ == 8) && has_octal_prefix()) {
            base_ = 8;
            eat_octal_prefix();
        } else if ((base_ == 0 || base_ == 16) && has_hex_prefix()) {
            base_ = 16;
            eat_hex_prefix();
        } else if (base_ == 0) {
            base_ = 10;
        }
    }

    inline bool has_octal_prefix()
    {
        return (*str_ == '0') && *(str_+1) != 'x';
    }

    inline void eat_octal_prefix()
    {
        str_++;
    }

    inline bool has_hex_prefix()
    {
        return (*str_ == '0' && *(str_+1) == 'x');
    }

    inline void eat_hex_prefix()
    {
        str_+=2;
    }

    const char* scan_to_end_of_number(const char* str)
    {
        while(is_valid_digit_for_char(*str)) {
            str++;
        }
        return str;
    }

    inline bool is_valid_digit_for_char(int c)
    {
        return is_alphanumeric(c) && (get_digit_for_char(c) < base_);
    }

    inline bool is_alphanumeric(int c)
    {
        return isdigit(c) || islower(c) || isupper(c);
    }

    long parse_number_digits(const char* endptr)
    {
        long accum = 0;
        int exponent = 0;

        for (const char* p = endptr - 1;
            is_valid_digit_for_char(*p) && (p >= str_);
            --p) {
            
            long value = get_digit_for_char(*p) * pow_long(exponent);
            accum += value;
            exponent++;
        }

        return accum;
    }

    long get_digit_for_char(int c)
    {
        if (isdigit(c)) {
            return c - '0';
        } else if (islower(c)) {
            return (c - 'a') + 10;
        } else if (isupper(c)) {
            return (c - 'A') + 10;
        } else {
            return 0;
        }
    }

    long pow_long(int exponent)
    {
        long value = 1;
        while (exponent-- > 0) {
            value *= base_;
        }
        return value;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_FLAPJACKLIBC_STRING_TO_LONG_CONVERTER_H
