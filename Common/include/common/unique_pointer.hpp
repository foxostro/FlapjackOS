#ifndef FLAPJACKOS_COMMON_INCLUDE_UNIQUE_POINTER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_UNIQUE_POINTER_HPP

#include <common/interrupt_lock.hpp>
#include <common/lock_guard.hpp>
#include <cassert>

// Unique Pointer class, roughly equivalent to std::unique_pointer.
template<typename Type, typename LockType = InterruptLock>
class UniquePointer {
public:
    ~UniquePointer()
    {
        release_unlocked();
    }

    UniquePointer() : UniquePointer(nullptr) {}

    UniquePointer(Type* data) : data_(data), should_leak_(false) {}

    UniquePointer(const UniquePointer& other) = delete;

    UniquePointer(UniquePointer&& other)
    {
        LockGuard guard{other.lock_};
        should_leak_ = other.should_leak_;
        data_ = other.data_;
        other.data_ = nullptr;
        other.should_leak_ = false;
    }

    Type& operator*() const
    {
        return *get_pointer();
    }

    Type* operator->() const
    {
        return get_pointer();
    }

    Type* get_pointer() const
    {
        LockGuard guard{lock_};
        return data_;
    }

    UniquePointer& operator=(const UniquePointer& other) = delete;

    UniquePointer& operator=(UniquePointer&& other)
    {
        if (this != &other) {
            LockGuard guard{lock_, other.lock_};
            release_unlocked();
            data_ = other.data_;
            should_leak_ = other.should_leak_;
            other.data_ = nullptr;
            other.should_leak_ = false;
        }
        return *this;
    }

    // Honestly, two instances of UniquePointer should never be equal as that
    // would violate uniqueness.
    bool operator==(const UniquePointer& other)
    {
        if (this == &other) {
            return true;
        }

        LockGuard guard{lock_, other.lock_};
        return (data_ == other.data_);
    }

    inline bool operator!=(const UniquePointer& other)
    {
        return !(*this == other);
    }

    explicit operator bool() const
    {
        LockGuard guard{lock_};
        return (data_ != nullptr);
    }

    // Specifies that the object is to be intentionally leaked in the dtor.
    // This is useful when the object was not allocated on the heap.
    void set_should_leak()
    {
        LockGuard guard{lock_};
        should_leak_ = true;
    }
    
private:
    Type* data_;
    mutable LockType lock_;
    bool should_leak_;

    void release_unlocked()
    {
        if (!should_leak_) {
            delete data_;
        }
        data_ = nullptr;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_UNIQUE_POINTER_HPP
