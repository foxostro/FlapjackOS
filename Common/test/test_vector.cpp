#include "catch.hpp"

#include <common/vector.hpp>

TEST_CASE("Initialize empty vector", "[Vector]")
{
    vector<int> v;
    REQUIRE(v.size() == 0);
    REQUIRE(v.capacity() == 0);
    REQUIRE(v.empty());
}

TEST_CASE("Initialize vector with default-constructed elements", "[Vector]")
{
    vector<int> v(100);
    REQUIRE(v.size() == 100);
    REQUIRE(v.capacity() >= 100);
    REQUIRE(!v.empty());
}

TEST_CASE("Initialize vector with array", "[Vector]")
{
    static const int arr[3] = {0, 1, 2};

    vector<int> v(3, arr);
    REQUIRE(v.size() == 3);
    REQUIRE(v.at(0) == arr[0]);
    REQUIRE(v.at(1) == arr[1]);
    REQUIRE(v.at(2) == arr[2]);
}

TEST_CASE("Copy constructor", "[Vector]")
{
    vector<int> a;
    a.push_back(0);
    a.push_back(1);
    a.push_back(2);

    vector<int> b(a);
    REQUIRE(a.at(0) == b.at(0));
    REQUIRE(a.at(1) == b.at(1));
    REQUIRE(a.at(2) == b.at(2));
    REQUIRE(a.size() == b.size());
}

TEST_CASE("Copy-assignment operator", "[Vector]")
{
    vector<int> a;
    a.push_back(0);
    a.push_back(1);
    a.push_back(2);

    vector<int> b(a);
    b.push_back(5);
    b.push_back(6);
    b.push_back(7);

    REQUIRE(b != a);

    b = a;

    REQUIRE(b == a);
}

TEST_CASE("Move constructor", "[Vector]")
{
    vector<char> a;
    a.push_back('a');
    a.push_back('b');
    a.push_back('c');

    const char *ptr = a.data();

    vector<char> b(std::move(a));
    REQUIRE('a' == b.at(0));
    REQUIRE('b' == b.at(1));
    REQUIRE('c' == b.at(2));
    REQUIRE(3 == b.size());
    REQUIRE(ptr == b.data());

    REQUIRE(0 == a.capacity());
    REQUIRE(0 == a.size());
    REQUIRE(nullptr == a.data());
}

TEST_CASE("Move-assignment operator", "[Vector]")
{
    vector<char> a;
    a.push_back('a');
    a.push_back('b');
    a.push_back('c');

    const char *ptr = a.data();

    vector<char> b;
    b.push_back('x');

    b = std::move(a);

    REQUIRE('a' == b.at(0));
    REQUIRE('b' == b.at(1));
    REQUIRE('c' == b.at(2));
    REQUIRE(3 == b.size());
    REQUIRE(ptr == b.data());

    REQUIRE(0 == a.capacity());
    REQUIRE(0 == a.size());
    REQUIRE(nullptr == a.data());
}

TEST_CASE("Equality operators", "[Vector]")
{
    vector<char> a;
    a.push_back('a');
    a.push_back('b');
    a.push_back('c');

    vector<char> b;
    b.push_back('a');
    b.push_back('b');
    b.push_back('c');

    vector<char> c;
    c.push_back('w');
    c.push_back('x');
    c.push_back('y');
    c.push_back('z');

    REQUIRE(a == a);
    REQUIRE(a == b);
    REQUIRE(!(a == c));
    REQUIRE(a != c);
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

TEST_CASE("Reserve memory to avoid allocations", "[Vector]")
{
    constexpr int N = 10;
    
    vector<int> v;

    v.reserve(N);
    int *ptr = v.data();
    int capacity = v.capacity();

    for (int i = 0; i < N; ++i) {
        v.push_front(i);
    }

    REQUIRE(capacity == v.capacity());
    REQUIRE(ptr == v.data());
}

TEST_CASE("Remove elements from the front of the vector", "[Vector]")
{
    vector<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.remove(0);

    REQUIRE(v.at(0) == 1);
    REQUIRE(v.at(1) == 2);
    REQUIRE(v.size() == 2);
}

TEST_CASE("Remove elements from the back of the vector", "[Vector]")
{
    vector<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.remove(2);

    REQUIRE(v.at(0) == 0);
    REQUIRE(v.at(1) == 1);
    REQUIRE(v.size() == 2);
}

TEST_CASE("Remove elements from the middle of the vector", "[Vector]")
{
    vector<int> v;

    v.push_back(0);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.remove(1);

    REQUIRE(v.at(0) == 0);
    REQUIRE(v.at(1) == 2);
    REQUIRE(v.at(2) == 3);
    REQUIRE(v.size() == 3);
}