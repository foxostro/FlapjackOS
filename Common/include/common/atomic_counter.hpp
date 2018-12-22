#ifndef FLAPJACKOS_COMMON_INCLUDE_ATOMIC_COUNTER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_ATOMIC_COUNTER_HPP

// Counter which can be incremented and decremented atomically.
template<typename NumericType>
class AtomicCounter {
public:
    AtomicCounter() : count_(0) {}

    explicit AtomicCounter(NumericType initial_value)
     : count_(initial_value)
    {}

    AtomicCounter(const AtomicCounter& other)
    {
        __atomic_store(&count_, other.count_, __ATOMIC_SEQ_CST);
    }

    AtomicCounter& operator=(const AtomicCounter& other)
    {
        if (this != &other) {
            __atomic_store(&count_, other.count_, __ATOMIC_SEQ_CST);
        }
        return *this;
    }

    int get_value() const
    {
        return __atomic_load_n(&count_, __ATOMIC_SEQ_CST);
    }

    NumericType increment()
    {
        return fetch_add(1);
    }

    NumericType decrement()
    {
        return fetch_sub(1);
    }

    NumericType fetch_add(NumericType value)
    {
        return __atomic_fetch_add(&count_, value, __ATOMIC_SEQ_CST);
    }

    NumericType fetch_sub(NumericType value)
    {
        return __atomic_fetch_sub(&count_, value, __ATOMIC_SEQ_CST);
    }

private:
    NumericType count_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_ATOMIC_COUNTER_HPP
