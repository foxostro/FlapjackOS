#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_VECTOR_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_VECTOR_HPP

#include <cstddef>
#include <cstdint>
#include <cstring>
#include <cassert>
#include <utility> // for std::move()
#include <type_traits> // for std::aligned_storage
#include <new> // for placement-new

template<typename T>
class Vector {
public:
    using value_type = T;
    using size_type = int;
    using internal_type = typename std::aligned_storage<sizeof(value_type), alignof(value_type)>::type;

    ~Vector()
    {
        if (data_) {
            delete[] data_;
        }
    }

    // Default constructor. Initializes an empty vector.
    Vector()
     : data_(nullptr), count_(0), capacity_(0)
    {}

    // Allocates memory for a vector of the specified size.
    Vector(size_type size)
     : data_(new internal_type[size]), count_(size), capacity_(size)
    {
        for (size_type i = 0; i < count_; ++i) {
            new (data() + i) value_type();
        }
    }

    // Copies the provided items into the vector.
    Vector(size_type size, const value_type *src)
     : data_(nullptr), count_(size), capacity_(size)
    {
        data_ = new internal_type[capacity_];
        for (size_type i = 0; i < count_; ++i) {
            (value_type &)(data_[i]) = src[i];
        }
    }

    // Copy constructor.
    Vector(const Vector &other)
     : Vector(other.count_, other.data())
    {}

    // Move constructor.
    Vector(Vector &&other)
     : data_(other.data_),
       count_(other.count_),
       capacity_(other.capacity_)
    {
        other.data_ = nullptr;
        other.count_ = 0;
        other.capacity_ = 0;
    }

    // Copy-assignment operator.
    Vector& operator=(const Vector &other)
    {
        if (this != &other) {
            if (data_) {
                delete[] data_;
            }

            count_ = other.count_;
            capacity_ = count_;
            data_ = new internal_type[capacity_];

            for (size_type i = 0; i < count_; ++i) {
                (value_type &)(data_[i]) = (value_type &)(other.data_[i]);
            }
        }

        return *this;
    }

    // Move-assignment operator.
    Vector& operator=(Vector &&other)
    {
        if (this != &other) {
            if (data_) {
                delete[] data_;
            }

            data_ = other.data_;
            count_ = other.count_;
            capacity_ = other.capacity_;

            other.data_ = nullptr;
            other.count_ = 0;
            other.capacity_ = 0;
        }

        return *this;
    }

    // Equality operator.
    bool operator==(const Vector &other) const
    {
        if (this == &other) {
            return true;
        }

        if (other.size() != size()) {
            return false;
        }

        for (size_type i = 0, n = size(); i < n; ++i) {
            if (other.at(i) != at(i)) {
                return false;
            }
        }

        return true;
    }

    // Not-equality operator.
    bool operator!=(const Vector &other) const
    {
        return !(*this == other);
    }

    // Returns the address of the first element in memory.
    // The vector gaurantees that all elements are contiguous in memory.
    const value_type* data() const
    {
        return (value_type *)data_;
    }

    // Returns the address of the first element in memory.
    // The vector gaurantees that all elements are contiguous in memory.
    value_type* data()
    {
        return (value_type *)data_;
    }

    // Returns the number of items in the collection.
    size_type size() const
    {
        return count_;
    }

    // Returns true if the collection is empty.
    bool empty() const
    {
        return count_ == 0;
    }

    // Returns the number of elements for which space is currently allocated.
    size_type capacity() const
    {
        return capacity_;
    }

    // Gets the index'th element of the collection.
    value_type& at(size_type i)
    {
        assert(i >= 0 && i < count_);
        return data()[i];
    }

    // Gets the index'th element of the collection.
    value_type& operator[](size_type i)
    {
        return at(i);
    }

    // Gets the index'th element of the collection.
    value_type at(size_type i) const
    {
        assert(i >= 0 && i < count_);
        return data()[i];
    }

    // Gets the index'th element of the collection.
    value_type operator[](size_type i) const
    {
        return at(i);
    }

    // Gets the first element.
    value_type& front()
    {
        assert(!empty());
        return at(0);
    }

    // Gets the last element.
    value_type& back()
    {
        assert(!empty());
        return at(size()-1);
    }

    // Increases the capacity of the vector such that at least the specified
    // number of elements may be inserted before additional allocations occur.
    void reserve(size_type new_capacity)
    {
        assert(new_capacity >= 0);

        if(new_capacity > capacity_) {
            internal_type *new_data = new internal_type[new_capacity];

            for (size_type i = 0; i < count_; ++i) {
                new ((value_type *)new_data + i) value_type(std::move(data()[i]));
            }

            if (data_) {
                delete[] data_;
            }
            data_ = new_data;
            capacity_ = new_capacity;
        }
    }

    // Inserts an item at the end of the collection.
    void push_back(value_type value)
    {
        insert(count_, value);
    }

    // Inserts an item at the beginning of the collection.
    // All existing items are moved to make room at the beginning.
    void push_front(value_type value)
    {
        insert(0, value);
    }

    // Inserts an item at the specified index of the collection.
    // All existing items are moved toward the back to make room.
    void insert(size_type index, value_type value)
    {
        assert(index >= 0 && index <= count_);

        increase_capacity_for_insertion();

        for (size_type i = count_; i > index; --i) {
            value_type &prev = data()[i-1];
            new (data() + i) value_type(std::move(prev));
            prev.~value_type();
        }

        new (data() + index) value_type(value);
        count_++;
    }

    // Removes the index'th element of the collection.
    // Returns the value of the removed element.
    // TODO: Change return value to "void" to avoid copying the old value.
    value_type remove(size_type index)
    {
        assert(index >= 0 && index < count_);

        value_type old_value = data()[index];

        for (size_type i = index; i < count_-1; ++i) {
            value_type &curr = data()[i];
            curr.~value_type();

            value_type &next = data()[i+1];
            new (data() + i) value_type(std::move(next));
        }

        count_--;

        return old_value;
    }

    // Removes the last element of the collection.
    // Returns the value of the removed element.
    // TODO: Change return value to "void" to avoid copying the old value.
    value_type pop_back()
    {
        return remove(size()-1);
    }

    // Removes the first element of the collection.
    // Returns the value of the removed element.
    // TODO: Change return value to "void" to avoid copying the old value.
    value_type pop_front()
    {
        return remove(0);
    }

private:
    static constexpr unsigned GROWTH_FACTOR = 2;

    internal_type *data_;
    size_type count_, capacity_;

    void increase_capacity_for_insertion()
    {
        if (capacity_ == 0) {
            reserve(1);
        } else if (count_ == capacity_) {
            reserve(count_ * GROWTH_FACTOR);
        }
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_VECTOR_HPP
