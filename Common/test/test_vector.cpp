#include "catch.hpp"

#include <common/vector.hpp>

TEST_CASE("Initialize empty vector", "[Vector]")
{
    vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
    REQUIRE(v.empty());
}

TEST_CASE("Append to end of vector", "[Vector]")
{
    constexpr int N = 10;

    vector<int> v;

    for (int i = 0; i < N; ++i) {
        REQUIRE(v.size() == i);
        v.push_back(i);
        REQUIRE(v.size() == (i+1));

        for (int j = 0; j < i; ++j) {
            REQUIRE(v.at(j) == j);
        }
    }
}

TEST_CASE("Append to front of vector", "[Vector]")
{
    constexpr int N = 10;
    
    vector<int> v;

    for (int i = 0; i < N; ++i) {
        REQUIRE(v.size() == i);
        v.push_front(i);
        REQUIRE(v.size() == (i+1));

        for (int j = 0; j < i; ++j) {
            REQUIRE(v.at(j) == (i-j));
        }
    }
}