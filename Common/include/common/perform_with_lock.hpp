#ifndef FLAPJACKOS_COMMON_INCLUDE_PERFORM_WITH_LOCK_HPP
#define FLAPJACKOS_COMMON_INCLUDE_PERFORM_WITH_LOCK_HPP

#include <common/lock_guard.hpp>

// AFOX_TODO: perform_with_lock() doesn't really work when attempting to return a reference. I get errors like "error: returning reference to local temporary object [-Werror,-Wreturn-stack-address]". I think I can fix this with some magic involving forwarding references or universal references, but I'm not sure.
template<typename LockType, typename Function>
inline auto perform_with_lock(LockType& lock, Function&& fn)
{
    LockGuard guard{lock};
    return fn();
}

template<typename Lock1, typename Lock2, typename Function>
inline auto perform_with_lock(Lock1& lock1, Lock2& lock2, Function&& fn)
{
    LockGuard2 guard{lock1, lock2};
    return fn();
}

#endif // FLAPJACKOS_COMMON_INCLUDE_PERFORM_WITH_LOCK_HPP
