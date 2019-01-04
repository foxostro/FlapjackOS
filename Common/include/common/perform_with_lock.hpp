#ifndef FLAPJACKOS_COMMON_INCLUDE_PERFORM_WITH_LOCK_HPP
#define FLAPJACKOS_COMMON_INCLUDE_PERFORM_WITH_LOCK_HPP

#include <common/lock_guard.hpp>

template<typename LockType, typename Function>
inline auto perform_with_lock(LockType& lock, Function&& fn) -> decltype(fn())
{
    LockGuard guard{lock};
    return fn();
}

// AFOX_TODO: Use variadic templates to generalize perform_with_lock() to N locks.
template<typename Lock1, typename Lock2, typename Function>
inline auto perform_with_lock(Lock1& lock1, Lock2& lock2, Function&& fn) -> decltype(fn())
{
    // AFOX_TODO: Use variadic templates to generalize LockGuard to N locks.
    LockGuard2 guard{lock1, lock2};
    return fn();
}

#endif // FLAPJACKOS_COMMON_INCLUDE_PERFORM_WITH_LOCK_HPP
