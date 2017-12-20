#pragma once

#include <cstddef>
#include <cassert>

template<typename Type>
class linked_list {
    struct linked_list_node {
        linked_list_node *prev, *next;
        Type value;

        linked_list_node(const Type &val)
         : prev(nullptr), next(nullptr), value(val)
        {}
    };

    linked_list_node *_head;
    int _count;

    // Gets the index'th node after `head'.
    // Returns the head if index is zero.
    linked_list_node* node_at(linked_list_node *head, int index)
    {
        for (int i = 0; i < index; ++i) {
            assert(head && head->next);
            head = head->next;
        }
        return head;
    }

public:
    ~linked_list()
    {
        while (!empty()) {
            remove(0);
        }
    }

    linked_list() : _head(nullptr), _count(0) {}

    // Inserts an item at the tail of the linked list.
    void push_back(const Type &value)
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
    void push_front(const Type &value)
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
    Type& at(int index)
    {
        assert(index >= 0 && index < _count);
        linked_list_node *node = node_at(_head, index);
        assert(node);
        return node->value;
    }

    // Removes the index'th item in the list.
    // Returns the value of the removed item.
    Type remove(int index)
    {
        assert(index >= 0 && index < _count);
        assert(_count > 0);

        _count--;

        if (index == 0) {
            linked_list_node *node = _head;
            _head = _head->next;
            Type value = node->value;
            delete node;
            return value;
        } else {
            linked_list_node *node = node_at(_head, index);
            Type value = node->value;
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
    int count() const
    {
        return _count;
    }

    // Returns true if the list is empty.
    bool empty() const
    {
        return _count == 0;
    }
};
