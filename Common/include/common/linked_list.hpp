#pragma once

#include <cstddef>
#include <cstdint>
#include <cassert>

template<typename T>
class linked_list {
public:
    using value_type = T;
    using size_type = int;

    ~linked_list()
    {
        while (!empty()) {
            remove(0);
        }
    }

    linked_list() : _head(nullptr), _count(0) {}

    // Inserts an item at the tail of the linked list.
    void push_back(value_type value)
    {
        _count++;
        linked_list_node *node = new linked_list_node(value);
        if (_head) {
            linked_list_node *tail;
            for (tail = _head; tail->next; tail = tail->next);
            tail->next = node;
            node->prev = tail;
        } else {
            _head = node;
        }
    }

    // Inserts an item at the head of the linked list.
    void push_front(value_type value)
    {
        _count++;
        linked_list_node *node = new linked_list_node(value);
        if (_head) {
            _head->prev = node;
            node->next = _head;
        }
        _head = node;
    }

    // Gets the index'th value of the list.
    // Returns the value of the head if index is zero.
    value_type at(size_type index) const
    {
        assert(index >= 0 && index < _count);
        linked_list_node *node = node_at(_head, index);
        assert(node);
        return node->value;
    }

    value_type& operator[](size_type i)
    {
        return at(i);
    }

    // Gets the index'th value of the list.
    // Returns the value of the head if index is zero.
    value_type& at(size_type index)
    {
        assert(index >= 0 && index < _count);
        linked_list_node *node = node_at(_head, index);
        assert(node);
        return node->value;
    }

    value_type operator[](size_type i) const
    {
        return at(i);
    }

    // Removes the index'th item in the list.
    // Returns the value of the removed item.
    value_type remove(size_type index)
    {
        assert(index >= 0 && index < _count);
        assert(_count > 0);

        _count--;

        if (index == 0) {
            linked_list_node *node = _head;
            _head = _head->next;
            value_type value = node->value;
            delete node;
            return value;
        } else {
            linked_list_node *node = node_at(_head, index);
            value_type value = node->value;
            if (node->prev) {
                node->prev->next = node->next;
            }
            if (node->next) {
                node->next->prev = node->prev;
            }
            delete node;
            return value;
        }
    }

    // Returns the number of items in the list.
    size_type size() const
    {
        return _count;
    }

    // Returns true if the list is empty.
    bool empty() const
    {
        return _count == 0;
    }

private:
    struct linked_list_node {
        linked_list_node *prev, *next;
        value_type value;

        linked_list_node(const value_type &val)
         : prev(nullptr), next(nullptr), value(val)
        {}
    };

    linked_list_node *_head;
    size_type _count;

    // Gets the index'th node after `head'.
    // Returns the head if index is zero.
    linked_list_node* node_at(linked_list_node *head, size_type index)
    {
        for (size_type i = 0; i < index; ++i) {
            assert(head && head->next);
            head = head->next;
        }
        return head;
    }
};
