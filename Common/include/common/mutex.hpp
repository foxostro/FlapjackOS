#ifndef FLAPJACKOS_COMMON_INCLUDE_MUTEX_HPP
#define FLAPJACKOS_COMMON_INCLUDE_MUTEX_HPP

#include <atomic>

extern "C" void yield();

class Mutex {
public:
    explicit Mutex(bool value = false) : lock_(value) {}
    Mutex(const Mutex& other) = delete;
    Mutex(Mutex&& other) = delete;

    void lock()
    {
        while (lock_.exchange(true, std::memory_order_relaxed)) {
            yield();
        }
        std::atomic_thread_fence(std::memory_order_acquire);
    }

    void unlock()
    {
        std::atomic_thread_fence(std::memory_order_release);
        lock_.store(false, std::memory_order_relaxed);
    }

private:
    std::atomic<bool> lock_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_MUTEX_HPP
