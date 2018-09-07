#ifndef FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_CREG_H
#define FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_CREG_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uintptr_t get_cr2(void);
uintptr_t get_cr3(void);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNELPLATFORMSUPPORT_INCLUDE_CREG_H
