#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_EITHER_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_EITHER_HPP

struct Monostate {
    constexpr bool operator==(const Monostate&) const { return true; }
    constexpr bool operator!=(const Monostate&) const { return false; }
};

// This is basically a variant<L,R>. Unfortunately, we are not able to use
// std::variant on macOS prior to 10.14 due to changes in libc++.
template<typename L, typename R>
class Either {
public:
    using Left = L;
    using Right = R;

    static_assert(!std::is_void<Left>::value, "Left must not be void");
    static_assert(!std::is_void<Right>::value, "Right must not be void");

    ~Either()
    {
        destruct();
    }

    void destruct()
    {
        if (is_left_) {
            left_.~Left();
        } else {
            right_.~Right();
        }
    }

    Either(const Left& left) : is_left_(true)
    {
        new (&left_) Left(left);
    }

    Either(Left&& left) : is_left_(true)
    {
        new (&left_) Left(std::move(left));
    }
    
    Either(const Right& right) : is_left_(false)
    {
        new (&right_) Right(right);
    }

    Either(Right&& right) : is_left_(false)
    {
        new (&right_) Right(std::move(right));
    }
    
    Either(const Either& other) : is_left_(other.is_left_)
    {
        if (other.is_left_) {
            new (&left_) Left(other.left_);
        } else {
            new (&right_) Right(other.right_);
        }
    }
    
    Either(Either&& other) : is_left_(other.is_left_)
    {
        if (other.is_left_) {
            new (&left_) Left(std::move(other.left_));
        } else {
            new (&right_) Right(std::move(other.right_));
        }
    }

    Either& operator=(const Left& left)
    {
        destruct();
        is_left_ = true;
        new (&left_) Left(left);
        return *this;
    }

    Either& operator=(Left&& left)
    {
        destruct();
        is_left_ = true;
        new (&left_) Left(std::move(left));
        return *this;
    }

    Either& operator=(const Right& right)
    {
        destruct();
        is_left_ = false;
        new (&right_) Right(right);
        return *this;
    }

    Either& operator=(Right&& right)
    {
        destruct();
        is_left_ = false;
        new (&right_) Right(std::move(right));
        return *this;
    }

    Either& operator=(const Either& other)
    {
        if (this != &other) {
            destruct();
            is_left_ = other.is_left_;
            if (other.is_left_) {
                new (&left_) Left(other.left_);
            } else {
                new (&right_) Right(other.right_);
            }
        }
        return *this;
    }

    Either& operator=(Either&& other)
    {
        if (this != &other) {
            destruct();
            is_left_ = other.is_left_;
            if (other.is_left_) {
                new (&left_) Left(std::move(other.left_));
            } else {
                new (&right_) Right(std::move(other.right_));
            }
        }
        return *this;
    }

    bool is_left() const
    {
        return is_left_;
    }

    Left& get_left() &
    {
        assert(is_left());
        return left_;
    }

    Left&& get_left() &&
    {
        assert(is_left());
        return std::move(left_);
    }

    const Left& get_left() const&
    {
        assert(is_left());
        return left_;
    }

    Right& get_right() &
    {
        assert(!is_left());
        return right_;
    }

    Right&& get_right() &&
    {
        assert(!is_left());
        return std::move(right_);
    }

    const Right& get_right() const&
    {
        assert(!is_left());
        return right_;
    }
    
    bool operator==(const Left& left) const
    {
        if (is_left_) {
            return left_ == left;
        } else {
            return false;
        }
    }
    
    bool operator!=(const Left& left) const
    {
        if (is_left_) {
            return left_ != left;
        } else {
            return true;
        }
    }
    
    bool operator==(const Right& right) const
    {
        if (is_left_) {
            return false;
        } else {
            return right_ == right;
        }
    }
    
    bool operator!=(const Right& right) const
    {
        if (is_left_) {
            return true;
        } else {
            return right_ != right;
        }
    }
    
    bool operator==(const Either& other) const
    {
        if (is_left_ != other.is_left_) {
            return false;
        }
        if (is_left_ && other.is_left_) {
            return left_ == other.left_;
        } else {
            return right_ == other.right_;
        }
    }
    
    bool operator!=(const Either& other) const
    {
        if (is_left_ != other.is_left_) {
            return true;
        }
        if (is_left_ && other.is_left_) {
            return left_ != other.left_;
        } else {
            return right_ != other.right_;
        }
    }

private:
    union {
        Left left_;
        Right right_;
    };
    bool is_left_;
};

template<typename Self, typename Function>
auto map_impl(Self&& self, Function&& fn)
{
    using Value = typename std::decay<decltype(std::forward<Self>(self).get_value())>::type;
    if constexpr (std::is_same<Monostate, Value>::value) {
        if constexpr (std::is_void<decltype(fn())>::value) {
            return map_impl_monostate_returns_void(std::forward<Self>(self), std::forward<Function>(fn));
        } else {
            return map_impl_monostate_returns_nonvoid(std::forward<Self>(self), std::forward<Function>(fn));
        }
    } else {
        if constexpr (std::is_void<decltype(fn(std::forward<Self>(self).get_value()))>::value) {
            return map_impl_returns_void(std::forward<Self>(self), std::forward<Function>(fn));
        } else {
            return map_impl_returns_nonvoid(std::forward<Self>(self), std::forward<Function>(fn));
        }
    }
}

template<typename Self, typename Function>
auto map_impl_monostate_returns_void(Self&& self, Function&& fn)
{
    using Next = typename std::decay<Self>::type::template Next<Monostate>;
    if (self.has_value()) {
        fn();
        return Next(Monostate{});
    } else {
        return Next(self.get_either().get_left());
    }
}

template<typename Self, typename Function>
auto map_impl_monostate_returns_nonvoid(Self&& self, Function&& fn)
{
    using Next = typename std::decay<Self>::type::template Next<decltype(fn())>;
    if (self.has_value()) {
        return Next(fn());
    } else {
        return Next(self.get_either().get_left());
    }
}

template<typename Self, typename Function>
auto map_impl_returns_void(Self&& self, Function&& fn)
{
    using Next = typename std::decay<Self>::type::template Next<Monostate>;
    if (self.has_value()) {
        fn(std::forward<Self>(self).get_value());
        return Next(Monostate{});
    } else {
        return Next(self.get_either().get_left());
    }
}

template<typename Self, typename Function>
auto map_impl_returns_nonvoid(Self&& self, Function&& fn)
{
    using Next = typename std::decay<Self>::type::template Next<decltype(fn(std::forward<Self>(self).get_value()))>;
    if (self.has_value()) {
        return Next(fn(std::forward<Self>(self).get_value()));
    } else {
        return Next(self.get_either().get_left());
    }
}

template<typename Self, typename Function>
auto and_then_impl(Self&& self, Function&& fn)
{
    using Value = typename std::decay<decltype(std::forward<Self>(self).get_value())>::type;
    if constexpr (std::is_same<Monostate, Value>::value) {
        if constexpr (std::is_void<decltype(fn())>::value) {
            return map_impl_monostate_returns_void(std::forward<Self>(self), std::forward<Function>(fn));
        } else {
            return and_then_impl_monostate_returns_nonvoid(std::forward<Self>(self), std::forward<Function>(fn));
        }
    } else {
        if constexpr (std::is_void<decltype(fn(std::forward<Self>(self).get_value()))>::value) {
            return map_impl_returns_void(std::forward<Self>(self), std::forward<Function>(fn));
        } else {
            return and_then_impl_returns_nonvoid(std::forward<Self>(self), std::forward<Function>(fn));
        }
    }
}

template<typename Self, typename Function>
auto and_then_impl_monostate_returns_nonvoid(Self&& self, Function&& fn)
{
    using Next = decltype(fn());
    if (self.has_value()) {
        return fn();
    } else {
        return Next(self.get_either().get_left());
    }
}

template<typename Self, typename Function>
auto and_then_impl_returns_nonvoid(Self&& self, Function&& fn)
{
    using Next = decltype(fn(std::forward<Self>(self).get_value()));
    if (self.has_value()) {
        return fn(std::forward<Self>(self).get_value());
    } else {
        return Next(self.get_either().get_left());
    }
}

template<typename Self, typename Function>
auto or_else_impl(Self&& self, Function&& fn)
{
    if (false == self.has_value()) {
        fn();
    }
    return std::forward<Self>(self);
}

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_EITHER_HPP
