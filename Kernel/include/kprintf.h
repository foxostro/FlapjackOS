#pragma once

#include <stdio.h>
#include <console.h>

// Formats a string and prints directly to the VGA console.
size_t kprintf(const console_interface_t *console,
               const char * restrict fmt, ...);
