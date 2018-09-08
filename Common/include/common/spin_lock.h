#ifndef FLAPJACKOS_COMMON_INCLUDE_SPIN_LOCK_H
#define FLAPJACKOS_COMMON_INCLUDE_SPIN_LOCK_H

#include <common/spin_lock.h>

#ifdef __cplusplus
extern "C" {
#endif

void spin_lock(int *p);
void spin_unlock(int volatile *p);

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_SPIN_LOCK_H
