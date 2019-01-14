#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_OPTIONAL_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_OPTIONAL_HPP

#include <cassert>
#include "either.hpp"

struct None {
    constexpr bool operator==(const None&) const { return true; }
    constexpr bool operator!=(const None&) const { return false; }
};
static const None none;

template<typename T>
class Optional {
public:
    using Value = T;
    template<typename U> using Next = Optional<U>;

    static_assert(!std::is_void<T>::value,
                  "Optional<void> is not supported.");

    static_assert(!std::is_same<typename std::decay<T>::type, None>::value,
                  "Optional<None> is not supported.");
    
    Optional() : either_(none) {}
    Optional(const None&) : either_(none) {}
    Optional(const Optional& other) : either_(other.either_) {}
    Optional(Optional&& other) : either_(std::move(other.either_)) {}
    Optional(const Value& value) : either_(value) {}
    Optional(Value&& value) : either_(std::move(value)) {}

    Optional& operator=(const Value& value)
    {
        either_ = value;
        return *this;
    }

    Optional& operator=(Value&& value)
    {
        either_ = std::move(value);
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
            either_ = other.either_;
        }
        return *this;
    }

    Optional& operator=(Optional&& other)
    {
        if (this != &other) {
            either_ = std::move(other.either_);
        }
        return *this;
    }
    
    explicit operator bool() const
    {
        return has_value();
    }

    bool has_value() const
    {
        return !either_.is_left();
    }
    
    Value& operator*()
    {
        return get_value();
    }
    
    Value& operator*() const
    {
        return get_value();
    }

    Value* operator->()
    {
        assert(has_value());
        Value& ref = either_.get_right();
        return &ref;
    }

    Value* operator->() const
    {
        assert(has_value());
        Value& ref = either_.get_right();
        return &ref;
    }

    auto& get_value() & { return either_.get_right(); }
    auto&& get_value() && { return std::move(either_.get_right()); }
    const auto& get_value() const& { return either_.get_right(); }

    auto& get_either() & { return either_; }
    auto&& get_either() && { return std::move(either_); }
    const auto& get_either() const& { return either_; }

    void reset()
    {
        either_ = none;
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
        if (this == &other) {
            return true;
        }
        return either_ == other.either_;
    }
    
    bool operator!=(const Optional& other) const
    {
        if (this == &other) {
            return false;
        }
        return either_ != other.either_;
    }

    // Map functor.
    // If the optional is engaged then unwrap it, pass it to the given function,
    // wrap the function's return value in another optional, and return that.
    // If the optional is not engaged then return an empty optional with a type
    // matching the function.
    // A return value of void is handled by mapping to Monostate. Likewise,
    // no parameters are passed to `fn' in the case where Value=Monostate.
    // This method can be chained with other calls to map(), &c.
    template<typename Function>
    auto map(Function&& fn) &
    {
        return map_impl(*this, std::forward<Function>(fn));
    }

    template<typename Function>
    auto map(Function&& fn) &&
    {
        return map_impl(*this, std::forward<Function>(fn));
    }

    template<typename Function>
    auto map(Function&& fn) const&
    {
        return map_impl(*this, std::forward<Function>(fn));
    }

    // If the optional is engaged then unwrap it, pass it to the given function,
    // and then return whatever value that function returned.
    // If the optional is not engaged then return an empty optional with a type
    // matching the function.
    // A return value of void is handled by mapping to Monostate. Likewise,
    // no parameters are passed to `fn' in the case where Value=Monostate.
    // This method can be chained with other calls to map(), &c.
    template<typename Function>
    auto and_then(Function&& fn) &
    {
        return and_then_impl(*this, std::forward<Function>(fn));
    }

    template<typename Function>
    auto and_then(Function&& fn) &&
    {
        return and_then_impl(std::move(*this), std::forward<Function>(fn));
    }

    template<typename Function>
    auto and_then(Function&& fn) const&
    {
        return and_then_impl(*this, std::forward<Function>(fn));
    }

    // If the optional is disengaged then execute the function.
    // The function accepts no parameters and returns no value.
    // Since or_else() returns *this, it can be chained with map(), &c.
    template<typename Function>
    auto or_else(Function&& fn) &
    {
        return or_else_impl(*this, std::forward<Function>(fn));
    }
    
    template<typename Function>
    auto or_else(Function&& fn) &&
    {
        return or_else_impl(std::move(*this), std::forward<Function>(fn));
    }

    template<typename Function>
    auto or_else(Function&& fn) const&
    {
        return or_else_impl(*this, std::forward<Function>(fn));
    }
    
private:
    Either<None, Value> either_;
};

template<typename T, typename... Args> inline
Optional<T> make_optional(Args&&... args)
{
    return Optional<T>(T(std::forward<Args>(args)...));
}

template<typename T> inline
Optional<T> make_optional(T&& value)
{
    return Optional<T>(std::move(value));
}

template<typename T> inline
Optional<T> make_optional(const None& value)
{
    return Optional<T>(value);
}

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_OPTIONAL_HPP
