#pragma once

#ifdef __cplusplus
extern "C" {
#endif

#include <interfaces/console_interface.h>

// Returns the interface for the VGA console output driver.
void get_console_interface(console_interface_t *interface);

#ifdef __cplusplus
}
#endif