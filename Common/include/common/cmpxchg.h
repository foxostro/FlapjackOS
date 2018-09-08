#ifndef FLAPJACKOS_COMMON_INCLUDE_CMPXCHG_H
#define FLAPJACKOS_COMMON_INCLUDE_CMPXCHG_H

#include <common/spin_lock.h>

#ifdef __cplusplus
extern "C" {
#endif

int cmpxchg(int* pointer, int old_value, int new_value);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_CMPXCHG_H
