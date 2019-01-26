#ifndef FLAPJACKOS_COMMON_INCLUDE_PTHREAD_H
#define FLAPJACKOS_COMMON_INCLUDE_PTHREAD_H

// Minimal pthread stubs so that libcxxrt will build.

#ifdef __cplusplus
extern "C" {
#endif

#include "sched.h"

#define PTHREAD_ONCE_INIT (0)
#define PTHREAD_MUTEX_INITIALIZER (0)
#define PTHREAD_COND_INITIALIZER (0)

typedef int pthread_key_t;
typedef int pthread_once_t;
typedef int pthread_mutex_t;
typedef int pthread_mutexattr_t;
typedef int pthread_cond_t;

int pthread_key_create(pthread_key_t* key, void (*)(void*));
int pthread_once(pthread_once_t* control, void (*init)(void));
void* pthread_getspecific(pthread_key_t key);
int pthread_setspecific(pthread_key_t key, const void* data);
int pthread_mutex_init(pthread_mutex_t* mutex, const pthread_mutexattr_t*);
int pthread_mutex_lock(pthread_mutex_t* mutex);
int pthread_mutex_unlock(pthread_mutex_t* mutex);
int pthread_cond_wait(pthread_cond_t*, pthread_mutex_t*);
int pthread_cond_signal(pthread_cond_t*);

int* __errno();

#ifdef __cplusplus
}
#endif

#endif // FLAPJACKOS_COMMON_INCLUDE_PTHREAD_H