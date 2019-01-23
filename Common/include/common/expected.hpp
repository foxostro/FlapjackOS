#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_EXPECTED_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_EXPECTED_HPP

#include <cassert>
#include "either.hpp"
#include "error.hpp"

template<typename T>
class Expected {
public:
    using Value = T;
    template<typename U> using Next = Expected<U>;

    Expected(const Value& value) : either_(value) {}
    Expected(Value&& value) : either_(std::move(value)) {}
    Expected(const Error& error) : either_(error) {}
    Expected(Error&& error) : either_(std::move(error)) {}

    bool has_value() const { return !either_.is_left(); }

    Error& get_error() & { return either_.get_left(); }
    Error&& get_error() && { return std::move(either_.get_left()); }
    const Error& get_error() const& { return either_.get_left(); }

    Value& get_value() & { return either_.get_right(); }
    Value&& get_value() && { return std::move(either_.get_right()); }
    const Value& get_value() const& { return either_.get_right(); }

    auto& get_either() & { return either_; }
    auto&& get_either() && { return std::move(either_); }
    const auto& get_either() const& { return either_; }

    template<typename Function>
    auto map_error(Function&& fn) const
    {
        static_assert(!std::is_void<decltype(fn(get_error()))>::value, "");
        if (has_value()) {
            return *this;
        } else {
            return Expected{fn(get_error())};
        }
    }

    template<typename Function>
    Expected&& map_error(Function&& fn) &&
    {
        static_assert(!std::is_void<decltype(fn(get_error()))>::value, "");
        if (has_value()) {
            return std::move(*this);
        } else {
            return std::move(Expected{fn(get_error())});
        }
    }

    // Map functor.
    // If the expected has value then unwrap it, pass it to the given function,
    // wrap the function's return value in another expected, and return that.
    // If the expected has no value then return an empty optional with a type
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

    // If the expected has value then unwrap it, pass it to the given function,
    // and then return whatever value that function returned.
    // If the expected has no value then return an empty optional with a type
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

    // If the expected has no value then execute the function.
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
    Either<Error, Value> either_;
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_EXPECTED_HPP
