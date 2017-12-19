#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <stddef.h>
#include <interfaces/console_interface.h>

// Formats a string and prints directly to the VGA console.
size_t console_printf(const console_interface_t *console,
                      const char * restrict fmt, ...);

#ifdef __cplusplus
}
#endif
