#include "pthread.h"
#include <cassert>
#include <common/vector.hpp>
#include <common/mutex.hpp>
#include <common/lock_guard.hpp>

class ThreadSpecificDataStore {
public:
    using Key = size_t;
    using Value = void*;

    Key allocate_next_key()
    {
        LockGuard lock{mutex_};
        values_.push_back(Value{});
        return next_key_++;
    }

    Value get(Key key)
    {
        LockGuard lock{mutex_};
        return values_[key];
    }

    void set(Key key, Value value)
    {
        LockGuard lock{mutex_};
        values_[key] = value;
    }

private:
    Mutex mutex_;
    Key next_key_ = Key{};
    Vector<Value> values_;
} g_thread_specific_data_store;

extern "C"
int pthread_key_create(pthread_key_t* key,
                       void (*ignored)(void*) __attribute__((unused)))
{
    assert(key);
    *key = g_thread_specific_data_store.allocate_next_key();
    return 0;
}

extern "C"
int pthread_once(pthread_once_t* control, void (*init)())
{
    assert(control);
    assert(init);
    static Mutex mutex;
    LockGuard guard{mutex};
    if (*control == 0) {
        (*init)();
        *control = 1;
    }
    return 0;
}

extern "C"
void* pthread_getspecific(pthread_key_t key)
{
    return g_thread_specific_data_store.get(key);
}

extern "C"
int pthread_setspecific(pthread_key_t key, const void* data)
{
    g_thread_specific_data_store.set(key, (void*)data);
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
