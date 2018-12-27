#ifndef FLAPJACKOS_COMMON_INCLUDE_MUTEX_HPP
#define FLAPJACKOS_COMMON_INCLUDE_MUTEX_HPP

#include "spin_lock.hpp"
#include <atomic>

class Mutex {
public:
    static void (*yield)();
    explicit Mutex(bool value = false) : lock_(value) {}
    Mutex(const Mutex& other) = delete;
    Mutex(Mutex&& other) = delete;

    void lock()
    {
        while (lock_.exchange(true, std::memory_order_relaxed)) {
            if (yield) {
                yield();
            }
        }
        std::atomic_thread_fence(std::memory_order_acquire);
    }

    void unlock()
    {
        std::atomic_thread_fence(std::memory_order_release);
        lock_.store(false, std::memory_order_relaxed);

        // Yield when releasing the lock so that the scheduler's round robin
        // queue can ensure those waiting on this lock will eventually be
        // serviced.
        if (yield) {
            yield();
        }
    }

private:
    std::atomic<bool> lock_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_MUTEX_HPP
