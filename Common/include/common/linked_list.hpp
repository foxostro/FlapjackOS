#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_LINKED_LIST_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_LINKED_LIST_HPP

#include <cstddef>
#include <cstdint>
#include <cassert>

template<typename T>
class LinkedList {
public:
    using value_type = T;
    using size_type = int;

    ~LinkedList()
    {
        while (!empty()) {
            remove(0);
        }
    }

    LinkedList() : head_(nullptr), count_(0) {}

    // Inserts an item at the tail of the linked list.
    void push_back(value_type value)
    {
        count_++;
        LinkedListNode *node = new LinkedListNode(value);
        if (head_) {
            LinkedListNode *tail;
            for (tail = head_; tail->next; tail = tail->next);
            tail->next = node;
            node->prev = tail;
        } else {
            head_ = node;
        }
    }

    // Inserts an item at the head of the linked list.
    void push_front(value_type value)
    {
        count_++;
        LinkedListNode *node = new LinkedListNode(value);
        if (head_) {
            head_->prev = node;
            node->next = head_;
        }
        head_ = node;
    }

    // Gets the index'th value of the list.
    // Returns the value of the head if index is zero.
    value_type at(size_type index) const
    {
        assert(index >= 0 && index < count_);
        LinkedListNode *node = node_at(head_, index);
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
        assert(index >= 0 && index < count_);
        LinkedListNode *node = node_at(head_, index);
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
        assert(index >= 0 && index < count_);
        assert(count_ > 0);

        count_--;

        if (index == 0) {
            LinkedListNode *node = head_;
            head_ = head_->next;
            value_type value = node->value;
            delete node;
            return value;
        } else {
            LinkedListNode *node = node_at(head_, index);
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
        return count_;
    }

    // Returns true if the list is empty.
    bool empty() const
    {
        return count_ == 0;
    }

private:
    struct LinkedListNode {
        LinkedListNode *prev, *next;
        value_type value;

        LinkedListNode(const value_type &val)
         : prev(nullptr), next(nullptr), value(val)
        {}
    };

    LinkedListNode *head_;
    size_type count_;

    // Gets the index'th node after `head'.
    // Returns the head if index is zero.
    LinkedListNode* node_at(LinkedListNode *head, size_type index)
    {
        for (size_type i = 0; i < index; ++i) {
            assert(head && head->next);
            head = head->next;
        }
        return head;
    }
};

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_LINKED_LIST_HPP
