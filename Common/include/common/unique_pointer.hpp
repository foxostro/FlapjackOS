#ifndef FLAPJACKOS_COMMON_INCLUDE_UNIQUE_POINTER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_UNIQUE_POINTER_HPP

#include <common/mutex.hpp>
#include <cassert>

// Unique Pointer class, roughly equivalent to std::unique_pointer.
template<typename Type, typename LockType = Mutex>
class UniquePointer {
public:
    ~UniquePointer()
    {
        release_unlocked();
    }

    UniquePointer() : UniquePointer(nullptr) {}

    UniquePointer(Type* data) : data_(data) {}

    UniquePointer(const UniquePointer& other) = delete;

    UniquePointer(UniquePointer&& other)
    {
        perform_with_lock(other.lock_, [&]{
            data_ = other.data_;
            other.data_ = nullptr;
        });
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
        Type* data;
        perform_with_lock(lock_, [&]{
            data = data_;
        });
        return data;
    }

    UniquePointer& operator=(const UniquePointer& other) = delete;

    UniquePointer& operator=(UniquePointer&& other)
    {
        if (this != &other) {
            perform_with_lock(lock_, other.lock_, [&]{
                release_unlocked();
                data_ = other.data_;
                other.data_ = nullptr;
            });
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

        bool is_equal;
        perform_with_lock(lock_, other.lock_, [&]{
            is_equal = (data_ == other.data_);
        });
        return is_equal;
    }

    inline bool operator!=(const UniquePointer& other)
    {
        return !(*this == other);
    }

    explicit operator bool() const
    {
        bool is_nonnull;
        perform_with_lock(lock_, [&]{
            is_nonnull = (data_ != nullptr);
        });
        return is_nonnull;
    }
    
private:
    Type* data_;
    mutable LockType lock_;

    void release_unlocked()
    {
        delete data_;
        data_ = nullptr;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_UNIQUE_POINTER_HPP
