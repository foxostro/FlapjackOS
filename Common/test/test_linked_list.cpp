#include "catch.hpp"

#include <common/linked_list.hpp>

TEST_CASE("Initialize Empty List", "[LinkedList]")
{
    linked_list<int> list;
    REQUIRE(list.count() == 0);
    REQUIRE(list.empty());
}

TEST_CASE("push_back", "[LinkedList]")
{
    linked_list<int> list;
    REQUIRE(list.count() == 0);

    for (int i = 0; i < 10; ++i) {
        list.push_back(i);
        REQUIRE(list.count() == (i+1));
        REQUIRE(list.at(i) == i);
    }
}

TEST_CASE("push_front", "[LinkedList]")
{
    linked_list<int> list;
    REQUIRE(list.count() == 0);

    for (int i = 0; i < 10; ++i) {
        list.push_front(i);
        REQUIRE(list.count() == (i+1));
        REQUIRE(list.at(0) == i);
    }
}

TEST_CASE("Remove the head.", "[LinkedList]")
{
    constexpr int N = 10;

    linked_list<int> list;
    for (int i = 0; i < N; ++i) {
        list.push_back(i);
    }

    for (int i = 0; i < N; ++i) {
        REQUIRE(list.count() == (N-i));
        REQUIRE(list.at(0) == i);
        int r = list.remove(0);
        REQUIRE(r == i);
    }

    REQUIRE(list.empty());
}

TEST_CASE("Remove the tail.", "[LinkedList]")
{
    constexpr int N = 10;

    linked_list<int> list;
    for (int i = 0; i < N; ++i) {
        list.push_back(i);
    }

    for (int i = N-1; i >= 0; --i) {
        REQUIRE(list.count() == (i+1));
        REQUIRE(list.at(i) == i);
        int r = list.remove(i);
        REQUIRE(r == i);
    }

    REQUIRE(list.empty());
}

TEST_CASE("Remove the middle.", "[LinkedList]")
{
    linked_list<int> list;
    list.push_back(0);
    list.push_back(1);
    list.push_back(2);
    REQUIRE(list.count() == 3);

    int r = list.remove(1);
    REQUIRE(r == 1);

    REQUIRE(list.count() == 2);
    REQUIRE(list.at(0) == 0);
    REQUIRE(list.at(1) == 2);
}
