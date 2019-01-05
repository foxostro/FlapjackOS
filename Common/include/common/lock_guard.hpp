#ifndef FLAPJACKOS_COMMON_INCLUDE_LOCK_GUARD_HPP
#define FLAPJACKOS_COMMON_INCLUDE_LOCK_GUARD_HPP

namespace LockGuardDetail {

// See also <https://eli.thegreenplace.net/2014/variadic-templates-in-c/>

template <class... Ts> struct tuple {};

template <class T, class... Ts>
struct tuple<T, Ts...> : tuple<Ts...> {
    tuple(T& t, Ts&... ts)
    : tuple<Ts...>(ts...),
        lock_(t)
    {
        lock_.lock();
    }

    ~tuple()
    {
        lock_.unlock();
    }

    T& lock_;
};

} // namespace LockGuardDetail

template<typename... Locks>
class LockGuard {
public:
    LockGuard(Locks&... locks)
     : locks_(locks...)
    {}

    ~LockGuard()
    {}

private:
    LockGuardDetail::tuple<Locks&...> locks_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_LOCK_GUARD_HPP
