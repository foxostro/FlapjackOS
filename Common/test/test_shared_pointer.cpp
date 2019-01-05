#include "catch.hpp"

#include <common/shared_pointer.hpp>

class TestObject {
public:
    static int count;

    TestObject()
    {
        count++;
    }

    ~TestObject()
    {
        count--;
    }
};

int TestObject::count = 0;

TEST_CASE("SharedPointer basic sanity", "[SharedPointer]")
{
    SharedPointer<int> shared_pointer(new int{50});
    REQUIRE(true == (bool)shared_pointer);
    REQUIRE(*shared_pointer == 50);
    REQUIRE(shared_pointer.get_count() == 1);
    *shared_pointer = 42;
    REQUIRE(*shared_pointer == 42);
}

TEST_CASE("SharedPointer nullptr pointer", "[SharedPointer]")
{
    SharedPointer<int> shared_pointer(nullptr);
    REQUIRE(shared_pointer.get_count() == 0);
    REQUIRE(shared_pointer.get_pointer() == nullptr);
    REQUIRE(false == (bool)shared_pointer);
}

TEST_CASE("SharedPointer default ctor", "[SharedPointer]")
{
    SharedPointer<int> shared_pointer;
    REQUIRE(shared_pointer.get_count() == 0);
    REQUIRE(shared_pointer.get_pointer() == nullptr);
    REQUIRE(false == (bool)shared_pointer);
}

TEST_CASE("SharedPointer copy constructor", "[SharedPointer]")
{
    TestObject::count = 0;

    {
        SharedPointer<TestObject> ptr1(new TestObject);
        REQUIRE(ptr1.get_count() == 1);
        REQUIRE(TestObject::count == 1);

        {
            SharedPointer<TestObject> ptr2(ptr1);
            REQUIRE(ptr1.get_count() == 2);
            REQUIRE(ptr2.get_count() == 2);
            REQUIRE(TestObject::count == 1);
        }

        REQUIRE(ptr1.get_count() == 1);
        REQUIRE(TestObject::count == 1);
    }

    REQUIRE(TestObject::count == 0);
}

TEST_CASE("SharedPointer copy-assignment operator", "[SharedPointer]")
{
    TestObject::count = 0;

    {
        SharedPointer<TestObject> ptr1(new TestObject);
        REQUIRE(ptr1.get_count() == 1);
        REQUIRE(TestObject::count == 1);

        {
            SharedPointer<TestObject> ptr2;

            ptr2 = ptr1;

            REQUIRE(ptr1.get_count() == 2);
            REQUIRE(ptr2.get_count() == 2);
            REQUIRE(TestObject::count == 1);
        }

        REQUIRE(ptr1.get_count() == 1);
        REQUIRE(TestObject::count == 1);
    }

    REQUIRE(TestObject::count == 0);
}

TEST_CASE("SharedPointer move constructor", "[SharedPointer]")
{
    TestObject::count = 0;

    {
        SharedPointer<TestObject> ptr1(new TestObject);
        REQUIRE(ptr1.get_count() == 1);
        REQUIRE(TestObject::count == 1);

        SharedPointer<TestObject> ptr2(std::move(ptr1));

        REQUIRE(ptr1.get_count() == 0);
        REQUIRE(ptr2.get_count() == 1);
        REQUIRE(TestObject::count == 1);
    }

    REQUIRE(TestObject::count == 0);
}

TEST_CASE("SharedPointer move-assignment operator", "[SharedPointer]")
{
    TestObject::count = 0;

    {
        SharedPointer<TestObject> ptr1(new TestObject);
        SharedPointer<TestObject> ptr2;

        REQUIRE(ptr1.get_count() == 1);
        REQUIRE(ptr2.get_count() == 0);
        REQUIRE(TestObject::count == 1);

        ptr2 = std::move(ptr1);

        REQUIRE(ptr1.get_count() == 0);
        REQUIRE(ptr2.get_count() == 1);
        REQUIRE(TestObject::count == 1);
    }

    REQUIRE(TestObject::count == 0);
}

TEST_CASE("SharedPointer move from UniquePointer", "[SharedPointer]")
{
    TestObject::count = 0;

    {
        UniquePointer<TestObject> ptr1(new TestObject);
        SharedPointer<TestObject> ptr2;

        REQUIRE(ptr2.get_count() == 0);
        REQUIRE(TestObject::count == 1);

        ptr2 = std::move(ptr1);

        REQUIRE(ptr1.get_pointer() == nullptr);
        REQUIRE(ptr2.get_count() == 1);
        REQUIRE(TestObject::count == 1);
    }

    REQUIRE(TestObject::count == 0);
}