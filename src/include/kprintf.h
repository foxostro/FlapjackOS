#pragma once

#include <stdarg.h>

// Similar to traditional vsnprintf.
// Formats the string in manner similar to traditional vsnprintf and returns
// the result in `str'. 
//
// The `size' gives the number of characters that will be placed in `str' minus 
// one. And a terminating null character will be placed at the size-th position.
// This is intended to be the same as traditional vsnprintf.
//
// Returns the number of characters that would have been output if the size had
// not been limited.
int kvsnprintf(char * restrict str,
               size_t size,
               const char * restrict format,
               va_list args);

// Wraps kvsnprintf to allow the use of a variable number of arguments.
int ksnprintf(char * restrict str,
              size_t size,
              const char * restrict format,
              ...);
