#include "catch.hpp"
#include <cstdio>
#include <common/unique_pointer.hpp>
#include <common/either.hpp>

int counter = 0;
struct TestEitherCtorCount {
    ~TestEitherCtorCount()
    {
        counter--;
    }

    TestEitherCtorCount(int v) : value(v)
    {
        counter++;
    }

    TestEitherCtorCount(const TestEitherCtorCount& rhs) : value(rhs.value)
    {
        counter++;
    }
    
    TestEitherCtorCount(TestEitherCtorCount&& rhs)
    {
        value = rhs.value;
    }

    TestEitherCtorCount& operator=(TestEitherCtorCount&& rhs)
    {
        value = rhs.value;
        return *this;
    }

    TestEitherCtorCount& operator=(const TestEitherCtorCount& rhs)
    {
        value = rhs.value;
        return *this;
    }

    int value;
};

TEST_CASE("Either -- move an Either", "[Either]")
{
    counter = 0;
    const TestEitherCtorCount right{1};
    Either<char, TestEitherCtorCount> either1(right);

    Either<char, TestEitherCtorCount> either2 = std::move(either1);

    REQUIRE(counter == 2);
    REQUIRE(either2.is_left() == false);
    REQUIRE(either2.get_right().value == 1);
}

TEST_CASE("Either -- Right-Either with a move-only type", "[Either]")
{
    UniquePointer<int> value{new int{1000}};
    Either<char, UniquePointer<int>> either1(std::move(value));
    
    Either<char, UniquePointer<int>> either2 = std::move(either1);

    REQUIRE(either2.is_left() == false);
    REQUIRE(*either2.get_right() == 1000);
}

TEST_CASE("Either -- Left-Either with a move-only type", "[Either]")
{
    Either<char, UniquePointer<int>> either1(char{'a'});
    
    Either<char, UniquePointer<int>> either2 = std::move(either1);

    REQUIRE(either2.is_left() == true);
    REQUIRE(either2.get_left() == 'a');
}
