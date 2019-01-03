#ifndef FLAPJACKOS_COMMON_INCLUDE_LOCK_GUARD_HPP
#define FLAPJACKOS_COMMON_INCLUDE_LOCK_GUARD_HPP

template<typename Lock>
class LockGuard
{
public:
    LockGuard(Lock& lock)
     : lock_(lock)
    {
        lock_.lock();
    }

    ~LockGuard()
    {
        lock_.unlock();
    }

private:
    Lock& lock_;
};

template<typename Lock1, typename Lock2>
class LockGuard2
{
public:
    LockGuard2(Lock1& lock1, Lock2& lock2)
     : lock1_(lock1), lock2_(lock2)
    {
        lock1_.lock();
        lock2_.lock();
    }

    ~LockGuard2()
    {
        lock2_.unlock();
        lock1_.unlock();
    }

private:
    Lock1& lock1_;
    Lock2& lock2_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_LOCK_GUARD_HPP
