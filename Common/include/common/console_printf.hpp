#pragma once

#include <cstddef>

class console_device;

// Formats a string and prints directly to the VGA console.
size_t console_printf(console_device &console, const char *fmt, ...);
