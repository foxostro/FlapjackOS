#ifndef FLAPJACKOS_KERNEL_INCLUDE_GET_EBP_H
#define FLAPJACKOS_KERNEL_INCLUDE_GET_EBP_H

#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

uintptr_t* get_frame_pointer(void);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_KERNEL_INCLUDE_GET_EBP_H
