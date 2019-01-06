#ifndef FLAPJACKOS_COMMON_INCLUDE_SHARED_POINTER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_SHARED_POINTER_HPP

#include <common/mutex.hpp>
#include <common/lock_guard.hpp>
#include <common/unique_pointer.hpp>
#include <atomic>
#include <cassert>

// Shared Pointer class, roughly equivalent to std::shared_pointer.
template<typename Type, typename LockType = Mutex>
class SharedPointer {
public:
    ~SharedPointer()
    {
        release_unlocked();
    }

    SharedPointer() : SharedPointer(nullptr) {}

    SharedPointer(Type* data)
     : data_(data),
       count_(data ? new std::atomic<int>(1) : nullptr)
    {}

    SharedPointer(const SharedPointer& other)
    {
        LockGuard guard{other.lock_};
        data_ = other.data_;
        count_ = other.count_;
        acquire_unlocked();
    }

    SharedPointer(SharedPointer&& other)
    {
        LockGuard guard{other.lock_};
        data_ = other.data_;
        count_ = other.count_;
        other.data_ = nullptr;
        other.count_ = nullptr;
    }

    SharedPointer(UniquePointer<Type>&& other)
    {
        data_ = other.get_pointer();
        count_ = new std::atomic<int>(1);
        other.set_should_leak();
        other = nullptr;
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

    SharedPointer& operator=(const SharedPointer& other)
    {
        if (this != &other) {
            LockGuard guard{lock_, other.lock_};
            release_unlocked();
            data_ = other.data_;
            count_ = other.count_;
            acquire_unlocked();
        }
        return *this;
    }

    SharedPointer& operator=(SharedPointer&& other)
    {
        if (this != &other) {
            LockGuard guard{lock_, other.lock_};
            release_unlocked();
            data_ = other.data_;
            count_ = other.count_;
            other.data_ = nullptr;
            other.count_ = nullptr;
        }
        return *this;
    }

    SharedPointer& operator=(UniquePointer<Type>&& other)
    {
        LockGuard guard{lock_};
        release_unlocked();
        data_ = other.get_pointer();
        count_ = new std::atomic<int>(1);
        other.set_should_leak();
        other = nullptr;
        return *this;
    }

    bool operator==(const SharedPointer& other) const
    {
        if (this == &other) {
            return true;
        }

        LockGuard guard{lock_, other.lock_};
        return (data_ == other.data_);
    }

    inline bool operator!=(const SharedPointer& other) const
    {
        return !(*this == other);
    }

    explicit operator bool() const
    {
        LockGuard guard{lock_};
        return (data_ != nullptr);
    }

    int get_count() const
    {
        LockGuard guard{lock_};
        return (count_ ? count_->load() : 0);
    }
    
private:
    Type* data_;
    mutable std::atomic<int>* count_;
    mutable LockType lock_;

    void acquire_unlocked()
    {
        if (count_) {
            count_->fetch_add(1);
        }
    }

    void release_unlocked()
    {
        if (count_ && count_->fetch_sub(1) == 1) {
            delete data_;
            delete count_;
        }

        data_ = nullptr;
        count_ = nullptr;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_SHARED_POINTER_HPP
