#ifndef FLAPJACKOS_COMMON_INCLUDE_ATOMIC_COUNTER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_ATOMIC_COUNTER_HPP

#include <common/spin_lock.hpp>

// Counter which can be incremented and decremented atomically.
template<typename NumericType, typename LockType>
class AtomicCounter {
public:
    AtomicCounter() : count_(0) {}

    explicit AtomicCounter(NumericType initial_value)
     : count_(initial_value)
    {}

    AtomicCounter(const AtomicCounter& other)
    {
        other.lock_.lock();
        count_ = other.count_;
        other.lock_.unlock();
    }

    AtomicCounter& operator=(const AtomicCounter& other)
    {
        if (this != &other) {
            lock_.lock();
            other.lock_.lock();
            count_ = other.count_;
            other.lock_.unlock();
            lock_.unlock();
        }
        return *this;
    }

    bool operator==(const AtomicCounter& other)
    {
        if (this == &other) {
            return true;
        }

        bool is_equal;
        lock_.lock();
        other.lock_.lock();
        is_equal = (count_ == other.count_);
        other.lock_.unlock();
        lock_.unlock();
        return is_equal;
    }

    inline bool operator!=(const AtomicCounter& other)
    {
        return !(*this == other);
    }

    int get_value() const
    {
        int value;
        lock_.lock();
        value = count_;
        lock_.unlock();
        return value;
    }

    int increment()
    {
        int new_value;
        lock_.lock();
        count_++;
        new_value = count_;
        lock_.unlock();
        return new_value;
    }

    int decrement()
    {
        int new_value;
        lock_.lock();
        count_--;
        new_value = count_;
        lock_.unlock();
        return new_value;
    }

private:
    mutable LockType lock_;
    NumericType count_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_ATOMIC_COUNTER_HPP
