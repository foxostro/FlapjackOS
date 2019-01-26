#include "pthread.h"
#include <assert.h>

void* threadDataTable[64];
int freeEntry = 0;

extern "C"
int pthread_key_create(pthread_key_t* key,
                       void (*ignored)(void*) __attribute__((unused)))
{
    assert(freeEntry < 64);
 
    *key = freeEntry;
    freeEntry++;
    return 0;
}
 
extern "C"
int pthread_once(pthread_once_t* control, void (*init)(void))
{
    if (*control == 0) {
        (*init)();
        *control = 1;
    }
    return 0;
}
 
extern "C"
void* pthread_getspecific(pthread_key_t key)
{
    return threadDataTable[key];
}
 
extern "C"
int pthread_setspecific(pthread_key_t key, const void* data)
{
    threadDataTable[key] = (void*)data;
    return 0;
}
 
extern "C"
int pthread_mutex_init(pthread_mutex_t* mutex,
                       const pthread_mutexattr_t* attr __attribute__((unused)))
{
    *mutex = 0;
    return 0;
}
 
extern "C"
int pthread_mutex_lock(pthread_mutex_t* mutex)
{
    assert(*mutex == 0);
    *mutex = 1;
    return 0;
}
 
extern "C"
int pthread_mutex_unlock(pthread_mutex_t* mutex)
{
    assert(*mutex != 0);
    *mutex = 0;
    return 0;
}
 
extern "C"
int pthread_cond_wait(pthread_cond_t* cond __attribute__((unused)),
                      pthread_mutex_t* mutex __attribute__((unused)))
{
    return 0;
}
 
extern "C"
int pthread_cond_signal(pthread_cond_t* cond __attribute__((unused)))
{
    return 0;
}

extern "C" int* __errno()
{
    static int flapjack_errno = 0;
    return &flapjack_errno;
}
