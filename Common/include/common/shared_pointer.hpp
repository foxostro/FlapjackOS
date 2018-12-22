#ifndef FLAPJACKOS_COMMON_INCLUDE_SHARED_POINTER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_SHARED_POINTER_HPP

#include <common/spin_lock.hpp>
#include <atomic>
#include <cassert>

// Shared Pointer class implemented with a spin lock.
template<typename Type, typename LockType = SpinLock>
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
        perform_with_lock(other.lock_, [&]{
            data_ = other.data_;
            count_ = other.count_;
            acquire_unlocked();
        });
    }

    SharedPointer(SharedPointer&& other)
    {
        perform_with_lock(other.lock_, [&]{
            data_ = other.data_;
            count_ = other.count_;
            other.data_ = nullptr;
            other.count_ = nullptr;
        });
    }

    Type& operator*()
    {
        return *get_pointer();
    }

    const Type& operator*() const
    {
        return *get_pointer();
    }

    Type* operator->()
    {
        return get_pointer();
    }

    const Type* operator->() const
    {
        return get_pointer();
    }

    Type* get_pointer()
    {
        Type* data;
        perform_with_lock(lock_, [&]{
            data = data_;
        });
        return data;
    }

    const Type* get_pointer() const
    {
        Type* data;
        perform_with_lock(lock_, [&]{
            data = data_;
        });
        return data;
    }

    SharedPointer& operator=(const SharedPointer& other)
    {
        if (this != &other) {
            perform_with_lock(lock_, other.lock_, [&]{
                release_unlocked();
                data_ = other.data_;
                count_ = other.count_;
                acquire_unlocked();
            });
        }
        return *this;
    }

    SharedPointer& operator=(SharedPointer&& other)
    {
        if (this != &other) {
            perform_with_lock(lock_, other.lock_, [&]{
                release_unlocked();
                data_ = other.data_;
                count_ = other.count_;
                other.data_ = nullptr;
                other.count_ = nullptr;
            });
        }
        return *this;
    }

    bool operator==(const SharedPointer& other)
    {
        if (this == &other) {
            return true;
        }

        bool is_equal;
        perform_with_lock(lock_, other.lock_, [&]{
            is_equal = (data_ == other.data_);
        });
        return is_equal;
    }

    inline bool operator!=(const SharedPointer& other)
    {
        return !(*this == other);
    }

    operator bool() const
    {
        bool is_nonnull;
        perform_with_lock(lock_, [&]{
            is_nonnull = (data_ != nullptr);
        });
        return is_nonnull;
    }

    int get_count() const
    {
        int count;
        perform_with_lock(lock_, [&]{
            count = (count_ ? count_->load() : 0);
        });
        return count;
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
