#ifndef FLAPJACKOS_COMMON_INCLUDE_SPIN_LOCK_HPP
#define FLAPJACKOS_COMMON_INCLUDE_SPIN_LOCK_HPP

#include <common/spin_lock.h>

// Class to wrap a simple spin lock.
class SpinLock {
public:
    SpinLock() : lock_(0) {}
    SpinLock(const SpinLock& other) = delete;
    SpinLock(SpinLock&& other) = delete;

    void lock()
    {
        spin_lock(&lock_);
    }

    void unlock()
    {
        spin_unlock(&lock_);
    }

private:
    int lock_;
};

template<typename LockType, typename Function>
inline void perform_with_lock(LockType& lock, Function&& fn)
{
    lock.lock();
    fn();
    lock.unlock();
}

template<typename LockType, typename Function>
inline void perform_with_lock(LockType& lock1, LockType& lock2, Function&& fn)
{
    lock1.lock();
    lock2.lock();
    fn();
    lock2.unlock();
    lock1.unlock();
}

#endif // FLAPJACKOS_COMMON_INCLUDE_SPIN_LOCK_HPP
