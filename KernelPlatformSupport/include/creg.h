#pragma once

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uintptr_t get_cr2(void);
uintptr_t get_cr3(void);

#ifdef __cplusplus
}
#endif
