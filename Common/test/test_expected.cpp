#include "catch.hpp"
#include <cstdio>
#include <common/expected.hpp>
#include <common/shared_pointer.hpp>

TEST_CASE("Expected -- example", "[Expected]")
{
    using Image = int;
    using Cat = int;
    auto crop_to_cat = [](Image image) -> Expected<Cat> {
        return Expected<Cat>(image);
    };
    auto add_bow_tie = [](Cat cat){
        return Expected(cat+1);
    };
    auto make_eyes_sparkle = [](Cat cat){
        return Expected(cat+1);
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
            .map(add_rainbow)
            .or_else([]{ printf("error\n"); });
    };
    REQUIRE(get_cute_cat(Image{40}).get_value() == Cat{42});
}

TEST_CASE("Expected -- can construct an unexpected from an error", "[Expected]")
{
    Expected<int> foo{Error{-1}};
    REQUIRE(!foo.has_value());
    REQUIRE(foo.get_error().error_code == -1);
}

TEST_CASE("Expected -- can construct an expected from some value", "[Expected]")
{
    const int value = 1;
    Expected<int> foo{value};
    REQUIRE(foo.has_value());
    REQUIRE(foo.get_value() == value);
}

TEST_CASE("Expected -- can construct an expected from moving some r-value", "[Expected]")
{
    UniquePointer<int> pointer{new int{1}};
    Expected<UniquePointer<int>> foo{std::move(pointer)};
    REQUIRE(foo.has_value());
    REQUIRE(*foo.get_value() == 1);
}

TEST_CASE("Expected -- Call map() to apply a function to expected value", "[Expected]")
{
    const auto foo = Expected(2);
    auto bar = foo.map([](int value){
        REQUIRE(value == 2);
        return 'a';
    });
    static_assert(std::is_same<decltype(bar), Expected<char>>::value, "");
    REQUIRE(bar.has_value());
    REQUIRE(bar.get_value() == 'a');
}

TEST_CASE("Expected -- Calling map() does nothing to an unexpected", "[Expected]")
{
    auto foo = Expected<int>{Error{ENOMEM}};
    auto bar = foo.map([](int value){
        return value;
    });
    REQUIRE(!bar.has_value());
}

TEST_CASE("Expected -- Calls to map() can be chained.", "[Expected]")
{
    auto bar = Expected(0).map([](int value){
        return value+1;
    }).map([](int value){
        return value+1;
    });
    REQUIRE(bar.has_value());
    REQUIRE(bar.get_value() == 2);
}

TEST_CASE("Expected -- for a monostate expected, map's function accepts no parameters at all", "[Expected]")
{
    auto bar = Expected(Monostate{}).map([]{
        return 1;
    });
    REQUIRE(bar.has_value());
    REQUIRE(bar.get_value() == 1);
}

TEST_CASE("Expected -- when map's function returns void we map to a monostate expected", "[Expected]")
{
    int result = 0;
    auto foo = Expected(1).map([&result](int value){
        result = value;
    });
    static_assert(std::is_same<decltype(foo), Expected<Monostate>>::value, "");
    REQUIRE(result == 1);
    REQUIRE(foo.get_value() == Monostate{});
}

TEST_CASE("Expected -- when Expected<Monostate>::map's function returns void we map to Expected<Monostate>", "[Expected]")
{
    int result = 0;
    auto foo = Expected(Monostate{}).map([&result]{
        result = 1;
    });
    static_assert(std::is_same<decltype(foo), Expected<Monostate>>::value, "");
    REQUIRE(result == 1);
    REQUIRE(foo.get_value() == Monostate{});
}

TEST_CASE("Expected -- or_else() calls the function when the expected has error", "[Expected]")
{
    int result = 0;
    auto foo = Expected<int>{Error{ENOMEM}};
    auto bar = foo.or_else([&result](){
        result = 1;
    });
    REQUIRE(result == 1);
    REQUIRE(bar.get_error().error_code == ENOMEM);
}

TEST_CASE("Expected -- or_else() return the expected when the expected has an expected value", "[Expected]")
{
    int result = 0;
    auto foo = Expected<int>{42};
    auto bar = foo.or_else([&result](){
        result = 1;
    });
    REQUIRE(result == 0);
    REQUIRE(bar.has_value());
    REQUIRE(bar.get_value() == 42);
}

TEST_CASE("Expected -- or_else() can be chained", "[Expected]")
{
    int result = 0;
    auto foo = Expected<int>(Error{ENOMEM});
    auto bar = foo.or_else([&result](){
        result++;
    }).or_else([&result](){
        result++;
    });
    REQUIRE(result == 2);
    REQUIRE(!bar.has_value());
}

TEST_CASE("Expected -- and_then() calls the function when the Expected has expected value", "[Expected]")
{
    int result = 0;
    auto foo = Expected<int>{1}.and_then([&result](int value) {
        result += value;
        return Expected<int>{value};
    }).and_then([&result](int value) {
        result += value;
        return Expected<int>{result};
    });
    REQUIRE(foo.has_value());
    REQUIRE(foo.get_value() == 2);
    REQUIRE(result == 2);
}

TEST_CASE("Expected -- and_then() does nothing when the expected has error", "[Expected]")
{
    int result = 0;
    auto foo = Expected<int>{Error{ENOMEM}}.and_then([&result](int value){
        result = value;
        return Expected<int>{value};
    });
    REQUIRE(!foo.has_value());
    REQUIRE(result == 0);
}

TEST_CASE("Expected -- for a monostate expected, and_then's function accepts no parameters at all", "[Expected]")
{
    auto foo = Expected(Monostate{}).and_then([]{
        return Expected(1);
    });
    REQUIRE(foo.has_value());
    REQUIRE(foo.get_value() == 1);
}

TEST_CASE("Expected -- when and_then's function returns void we map to a monostate expected", "[Expected]")
{
    int result = 0;
    auto foo = Expected(1).and_then([&result](int value){
        result = value;
    });
    static_assert(std::is_same<decltype(foo), Expected<Monostate>>::value, "");
    REQUIRE(result == 1);
    REQUIRE(foo.get_value() == Monostate{});
}

TEST_CASE("Expected -- when Expected<Monostate>::and_then's function returns void we map to Expected<Monostate>", "[Expected]")
{
    int result = 0;
    auto foo = Expected(Monostate{}).and_then([&result]{
        result = 1;
    });
    static_assert(std::is_same<decltype(foo), Expected<Monostate>>::value, "");
    REQUIRE(result == 1);
    REQUIRE(foo.get_value() == Monostate{});
}

TEST_CASE("Expected -- or_else with Value=UniquePonter, a move-only type", "[Expected]")
{
    int result = -1;
    Expected<UniquePointer<int>>{Error{ENOMEM}}.or_else([&]{
        result = 1;
    });
    REQUIRE(result == 1);
}

TEST_CASE("Expected -- map_error() is like map() for the left side", "[Expected]")
{
    int out_error = 0;
    Expected<SharedPointer<int>> foo{Error{ENOMEM}};
    foo.map_error([&out_error](Error error){
        out_error = error.error_code;
        return error;
    });
    REQUIRE(out_error == ENOMEM);
    REQUIRE(!foo.has_value());
    REQUIRE(foo.get_error().error_code == ENOMEM);
}

TEST_CASE("Expected -- map_error with Value=UniquePointer, a move-only type", "[Expected]")
{
    int out_error = 0;
    Expected<UniquePointer<int>>{Error{ENOMEM}}.map_error([&out_error](Error error){
        out_error = error.error_code;
        return error;
    });
    REQUIRE(out_error == ENOMEM);
}

TEST_CASE("Expected -- and_then with Value=UniquePointer, a move-only type", "[Expected]")
{
    int result = 0;
    Expected<UniquePointer<int>>{new int{1}}.and_then([&](UniquePointer<int> p){
        result = 1;
        return Expected(std::move(p));
    });
    REQUIRE(result == 1);
}
