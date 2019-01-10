#include "catch.hpp"
#include <cstdio>
#include <common/optional.hpp>
#include <common/shared_pointer.hpp>

TEST_CASE("Optional -- example", "[Optional]")
{
    using Image = int;
    using Cat = int;
    auto crop_to_cat = [](Image image) -> Optional<Cat> {
        return MakeOptional<Cat>(image);
    };
    auto add_bow_tie = [](Cat cat){
        return MakeOptional(cat+1);
    };
    auto make_eyes_sparkle = [](Cat cat){
        return MakeOptional(cat+1);
    };
    auto make_smaller = [](Cat cat){
        return cat / 2;
    };
    auto add_rainbow = [](Cat cat){
        return cat * 2;
    };
    auto get_cute_cat = [&](Image image) {
        return crop_to_cat(image)
            .and_then(add_bow_tie)
            .and_then(make_eyes_sparkle)
            .map(make_smaller)
            .map(add_rainbow);
    };
    Image image = 40;
    REQUIRE(get_cute_cat(image).get_value() == 42);
}

TEST_CASE("Optional -- default constructed creates disengaged optional", "[Optional]")
{
    Optional<int> maybe;
    REQUIRE(!maybe.has_value());
}

TEST_CASE("Optional -- can construct a disengaged optional with 'none' as ctor parameter, #1", "[Optional]")
{
    Optional<int> foo{none};
    REQUIRE(!foo.has_value());
}

TEST_CASE("Optional -- can construct a disengaged optional with 'none' as ctor parameter, #2", "[Optional]")
{
    Optional<int> foo = none;
    REQUIRE(!foo.has_value());
}

TEST_CASE("Optional -- disengaged optional converts to false in a boolean context", "[Optional]")
{
    Optional<int> maybe;
    REQUIRE(static_cast<bool>(maybe) == false);
}

TEST_CASE("Optional -- engaged optional converts to true in a boolean context", "[Optional]")
{
    Optional<int> maybe = 2;
    REQUIRE(static_cast<bool>(maybe) == true);
}

TEST_CASE("Optional -- copy the value on construction", "[Optional]")
{
    auto value = SharedPointer(new int{42});
    Optional maybe(value);
    REQUIRE(value.get_count() == 2);
    REQUIRE(maybe.has_value());
    REQUIRE(maybe.get_value() == value);
}

TEST_CASE("Optional -- move the value on construction", "[Optional]")
{
    Optional maybe{UniquePointer(new int{42})};
    REQUIRE(maybe.has_value());
    REQUIRE(*maybe.get_value() == 42);
}

TEST_CASE("Optional -- copy-assign a value", "[Optional]")
{
    auto value = SharedPointer(new int{42});
    Optional<SharedPointer<int>> maybe;
    maybe = value;
    REQUIRE(value.get_count() == 2);
    REQUIRE(maybe.has_value());
    REQUIRE(*maybe.get_value() == 42);
}

TEST_CASE("Optional -- move-assign a value", "[Optional]")
{
    auto value = UniquePointer(new int{42});
    Optional<UniquePointer<int>> maybe;
    maybe = std::move(value);
    REQUIRE(!value);
    REQUIRE(maybe.has_value());
    REQUIRE(*maybe.get_value() == 42);
}

TEST_CASE("Optional -- move a value out of an optional", "[Optional]")
{
    auto maybe = Optional{UniquePointer(new int{42})};
    auto pointer = std::move(maybe.get_value());
    REQUIRE(*pointer == 42);

    // NOTE: The moved-from Optional is still engaged. However, the standard
    // states that a moved-from object is always left in an unspecified state.
    // So, this is permissable.
}

TEST_CASE("Optional -- copy-assign an optional", "[Optional]")
{
    Optional<SharedPointer<int>> foo{SharedPointer(new int{42})};
    Optional<SharedPointer<int>> bar;
    bar = foo;
    REQUIRE(foo.has_value());
    REQUIRE(bar.has_value());
    REQUIRE(*foo.get_value() == 42);
    REQUIRE(*bar.get_value() == 42);
    REQUIRE(foo.get_value() == bar.get_value());
    REQUIRE(foo.get_value().get_count() == 2);
}

TEST_CASE("Optional -- move-assign an optional", "[Optional]")
{
    Optional<SharedPointer<int>> foo{UniquePointer(new int{42})};
    Optional<SharedPointer<int>> bar;
    bar = std::move(foo);
    REQUIRE(!foo.has_value());
    REQUIRE(bar.has_value());
    REQUIRE(*bar.get_value() == 42);
}

class Foo {
public:
    static int count;

    ~Foo()
    {
        count--;
    }

    Foo([[maybe_unused]] int x, [[maybe_unused]] int y)
    {
        count++;
    }

    Foo()
    {
        count++;
    }

    Foo(const Foo&)
    {
        count++;
    }

    Foo(Foo&&)
    {
        count++;
    }

    Foo& operator=(const Foo&)
    {
        return *this;
    }

    Foo& operator=(Foo&&)
    {
        return *this;
    }

    int get_42() const
    {
        return 42;
    }
};

int Foo::count = 0;

TEST_CASE("Optional -- reset destroys the value", "[Optional]")
{
    Foo::count = 0;
    Optional<Foo> foo = Foo();
    foo.reset();
    REQUIRE(Foo::count == 0);
    REQUIRE(!foo.has_value());
}

TEST_CASE("Optional -- reset the optional by assigning 'none'", "[Optional]")
{
    Foo::count = 0;
    Optional<Foo> foo = Foo();
    foo = none;
    REQUIRE(Foo::count == 0);
    REQUIRE(!foo.has_value());
}

TEST_CASE("Optional -- dereference an optional to get the value", "[Optional]")
{
    Foo::count = 0;
    Optional<Foo> foo = Foo();
    REQUIRE(foo.has_value());
    Foo& bar = *foo;
    REQUIRE(bar.get_42() == 42);
    REQUIRE(Foo::count == 1);
}

TEST_CASE("Optional -- arrow operator", "[Optional]")
{
    Foo::count = 0;
    Optional<Foo> foo = Foo();
    REQUIRE(foo.has_value());
    REQUIRE(foo->get_42() == 42);
    REQUIRE(Foo::count == 1);
}

TEST_CASE("Optional -- emplace a value", "[Optional]")
{
    Optional<UniquePointer<int>> maybe;
    maybe.emplace(new int{1});
    REQUIRE(maybe.has_value());
    REQUIRE(**maybe == 1);
}

TEST_CASE("Optional -- MakeOptional emplaces a value", "[Optional]")
{
    Optional<Foo> maybe = MakeOptional<Foo>(1, 2);
    REQUIRE(maybe.has_value());
}

TEST_CASE("Optional -- MakeOptional can deduce the type", "[Optional]")
{
    auto maybe = MakeOptional(1u);
    REQUIRE(maybe.has_value());
    REQUIRE(maybe.get_value() == 1u);
    static_assert(std::is_same<decltype(maybe)::Type, unsigned int>::value,
                  "The Optional type should deduce to unsigned int.");
}

TEST_CASE("Optional -- MakeOptional(none) returns none", "[Optional]")
{
    Optional<Foo> maybe = MakeOptional<Foo>(none);
    REQUIRE(!maybe.has_value());
}

TEST_CASE("Optional -- A disengaged optional equals 'none'", "[Optional]")
{
    Optional<int> maybe;
    REQUIRE((maybe != none) == false);
    REQUIRE(maybe == none);
}

TEST_CASE("Optional -- An engaged optional does not equal 'none'", "[Optional]")
{
    auto maybe = MakeOptional(1);
    REQUIRE((maybe == none) == false);
    REQUIRE(maybe != none);
}

TEST_CASE("Optional -- An engaged optional is not equal to a disengaged optional", "[Optional]")
{
    Optional<int> foo;
    Optional<int> bar = 2;
    REQUIRE((foo == bar) == false);
    REQUIRE(foo != bar);
}

TEST_CASE("Optional -- Two engaged optional compare values, unequal", "[Optional]")
{
    Optional<int> foo = 1;
    Optional<int> bar = 2;
    REQUIRE((foo == bar) == false);
    REQUIRE(foo != bar);
}

TEST_CASE("Optional -- Two engaged optional compare values, equal", "[Optional]")
{
    Optional<int> foo = 2;
    Optional<int> bar = 2;
    REQUIRE((foo != bar) == false);
    REQUIRE(foo == bar);
}

TEST_CASE("Optional -- Call map() to apply a function to an engaged optional", "[Optional]")
{
    const auto foo = MakeOptional(2);
    auto bar = foo.map([](int value){
        REQUIRE(value == 2);
        return 'a';
    });
    static_assert(std::is_same<decltype(bar), Optional<char>>::value, "");
    REQUIRE(bar.has_value());
    REQUIRE(bar.get_value() == 'a');
}

TEST_CASE("Optional -- Calling map() does nothing to a disengaged optional", "[Optional]")
{
    auto foo = Optional<int>{none};
    auto bar = foo.map([](int value){
        return value;
    });
    REQUIRE(!bar.has_value());
}

TEST_CASE("Optional -- Calls to map() can be chained.", "[Optional]")
{
    auto bar = MakeOptional(0).map([](int value){
        return value+1;
    }).map([](int value){
        return value+1;
    });
    REQUIRE(bar.has_value());
    REQUIRE(bar.get_value() == 2);
}

TEST_CASE("Optional -- or_else() calls the function when the optional is disengaged", "[Optional]")
{
    int result = 0;
    auto foo = Optional<int>{none};
    auto bar = foo.or_else([&result](){
        result = 1;
    });
    REQUIRE(result == 1);
    REQUIRE(bar == none);
}

TEST_CASE("Optional -- or_else() return the optional when the optional is engaged", "[Optional]")
{
    int result = 0;
    auto foo = Optional<int>{42};
    auto bar = foo.or_else([&result](){
        result = 1;
    });
    REQUIRE(result == 0);
    REQUIRE(bar.has_value());
    REQUIRE(bar.get_value() == 42);
}

TEST_CASE("Optional -- or_else() can be chained", "[Optional]")
{
    int result = 0;
    auto foo = Optional<int>();
    auto bar = foo.or_else([&result](){
        result++;
    }).or_else([&result](){
        result++;
    });
    REQUIRE(result == 2);
    REQUIRE(!bar.has_value());
}

TEST_CASE("Optional -- and_then() calls the function when the optional is engaged", "[Optional]")
{
    int result = 0;
    auto foo = Optional<int>{1}.and_then([&result](int value) {
        result += value;
        return Optional<int>{value};
    }).and_then([&result](int value) {
        result += value;
        return Optional<int>{result};
    });
    REQUIRE(foo.has_value());
    REQUIRE(foo.get_value() == 2);
    REQUIRE(result == 2);
}

TEST_CASE("Optional -- and_then() does nothing when the optional is disengaged", "[Optional]")
{
    int result = 0;
    auto foo = Optional<int>{none}.and_then([&result](int value){
        result = value;
        return Optional<int>{value};
    });
    REQUIRE(!foo.has_value());
    REQUIRE(result == 0);
}
