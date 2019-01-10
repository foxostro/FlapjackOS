#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_OPTIONAL_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_OPTIONAL_HPP

#include <utility> // for std::move
#include <type_traits> // for std::aligned_storage
#include <new> // for placement-new
#include <cassert>

struct None {};
static const None none;

template<typename T>
class Optional {
public:
    using Type = T;

    static_assert(!std::is_void<Type>::value,
                  "Optional<void> is not supported.");

    static_assert(!std::is_same<typename std::decay<T>::type, None>::value,
                  "Optional<None> is not supported.");

    ~Optional()
    {
        reset();
    }
    
    Optional() : has_value_(false) {}

    Optional(const None&) : Optional() {}
    
    Optional(const Optional& other)
     : Optional()
    {
        *this = other;
    }
    
    Optional(Optional&& other)
     : Optional()
    {
        *this = std::move(other);
    }
    
    Optional(const Type& value)
     : pointer_(new (&storage_) Type(value)),
       has_value_(pointer_ != nullptr)
    {}

    Optional(Type&& value)
     : pointer_(new (&storage_) Type(std::move(value))),
       has_value_(pointer_ != nullptr)
    {}

    Optional& operator=(const Type& value)
    {
        pointer_ = new (&storage_) Type(value);
        has_value_ = (pointer_ != nullptr);
        return *this;
    }

    Optional& operator=(Type&& value)
    {
        pointer_ = new (&storage_) Type(std::move(value));
        has_value_ = (pointer_ != nullptr);
        return *this;
    }

    Optional& operator=(const None&)
    {
        reset();
        return *this;
    }

    Optional& operator=(const Optional& other)
    {
        if (this != &other) {
            reset();
            if (other.has_value_) {
                assert(other.pointer_);
                pointer_ = new (&storage_) Type(*other.pointer_);
                has_value_ = (pointer_ != nullptr);
            }
        }
        return *this;
    }

    Optional& operator=(Optional&& other)
    {
        if (this != &other) {
            reset();
            if (other.has_value_) {
                assert(other.pointer_);
                pointer_ = new (&storage_) Type(std::move(*other.pointer_));
                has_value_ = (pointer_ != nullptr);
                other.reset();
            }
        }
        return *this;
    }

    template <typename... Args>
    void emplace(Args&&... args)
    {
        reset();
        pointer_ = new (&storage_) Type(std::forward<Args>(args)...);
        has_value_ = (pointer_ != nullptr);
    }
    
    explicit operator bool() const
    {
        return has_value();
    }

    bool has_value() const
    {
        return has_value_;
    }
    
    Type& operator*()
    {
        return get_value();
    }
    
    Type& operator*() const
    {
        return get_value();
    }

    Type* operator->()
    {
        assert(has_value_);
        assert(pointer_);
        return pointer_;
    }

    Type* operator->() const
    {
        assert(has_value_);
        assert(pointer_);
        return pointer_;
    }

    const Type& get_value() const
    {
        assert(has_value_);
        assert(pointer_);
        return *pointer_;
    }

    Type& get_value()
    {
        assert(has_value_);
        assert(pointer_);
        return *pointer_;
    }

    void reset()
    {
        if (has_value_) {
            assert(pointer_);
            pointer_->~Type();
        }
        pointer_ = nullptr;
        has_value_ = false;
    }
    
    bool operator==(const None&) const
    {
        return !has_value();
    }
    
    bool operator!=(const None& none) const
    {
        return !(*this == none);
    }
    
    bool operator==(const Optional& other) const
    {
        if (has_value() != other.has_value()) {
            return false;
        }
        if (has_value() && other.has_value()) {
            return get_value() == other.get_value();
        }
        return true;
    }
    
    bool operator!=(const Optional& other) const
    {
        if (has_value() != other.has_value()) {
            return true;
        }
        if (has_value() && other.has_value()) {
            return get_value() != other.get_value();
        }
        return false;
    }

    // Map functor.
    // If the optional is engaged then unwrap it, pass it to the given function,
    // wrap the function's return value in another optional, and return that.
    // If the optional is not engaged then return an empty optional with a type
    // matching the function.
    // This method can be chained with other calls to map(), &c.
    template<typename Function>
    auto map(Function&& function) const -> Optional<decltype(function(get_value()))>
    {
        static_assert(!std::is_void<decltype(function(get_value()))>::value,
                      "The function must have a non-void return value.");
        if (has_value()) {
            return function(get_value());
        } else {
            return none;
        }
    }

    // If the optional is engaged then unwrap it, pass it to the given function,
    // and then return whatever value that function returned.
    // If the optional is not engaged then return an empty optional with a type
    // matching the function.
    // This method can be chained with other calls to map(), &c.
    template<typename Function>
    auto and_then(Function&& function) const -> decltype(function(get_value()))
    {
        using Return = decltype(function(get_value()));
        Return result;
        if (has_value()) {
            result = function(get_value());
        }
        return result;
    }

    // If the optional is disengaged then execute the function.
    // The function accepts no parameters and returns no value.
    // Since or_else() returns *this, it can be chained with map(), &c.
    template<typename Function>
    Optional<Type> or_else(Function&& function) const
    {
        if (!has_value()) {
            function();
        }
        return *this;
    }

private:
    using Storage = typename std::aligned_storage<sizeof(Type), alignof(Type)>::type;
    Storage storage_;
    Type* pointer_;
    bool has_value_;
};

template<typename T, typename... Args> inline
Optional<T> MakeOptional(Args&&... args)
{
    return Optional<T>(T(std::forward<Args>(args)...));
}

template<typename T> inline
Optional<T> MakeOptional(T&& value)
{
    return Optional<T>(value);
}

template<typename T> inline
Optional<T> MakeOptional(const None& value)
{
    return Optional<T>(value);
}

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_OPTIONAL_HPP
