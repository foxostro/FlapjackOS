// ll_str.c is an automatically generated file. Do not edit directly.




#include "ll_str.h"
#include <stdlib.h>
#include <assert.h>

ll_node_str_t * ll_node_alloc_str(char* value)
{
    ll_node_str_t *node = malloc(sizeof(ll_node_str_t));
    node->prev = NULL;
    node->next = NULL;
    node->value = value;
    return node;
}

void ll_node_add_str(ll_node_str_t *head, ll_node_str_t *node)
{
    assert(head);
    while (head->next) {
        head = head->next;
    }
    head->next = node;
    node->prev = head;
}

ll_node_str_t * ll_node_at_str(ll_node_str_t *head, size_t index)
{
    for (size_t i = 0; i < index; ++i) {
        assert(head && head->next);
        head = head->next;
    }
    return head;
}

char* ll_node_remove_str(ll_node_str_t *node)
{
    char* value = node->value;
    assert(node);
    if (node->prev) {
        node->prev->next = node->next;
    }
    if (node->next) {
        node->next->prev = node->prev;
    }
    free(node);
    return value;
}

ll_str_t * ll_init_str(void)
{
    ll_str_t *list = malloc(sizeof(ll_str_t));
    list->head = NULL;
    list->count = 0;
    return list;
}

void ll_destroy_str(ll_str_t *list)
{
    while (ll_count_str(list) > 0) {
        ll_remove_str(list, 0);
    }
    free(list);
}

void ll_push_back_str(ll_str_t *list, char* value)
{
    list->count++;
    ll_node_str_t *node = ll_node_alloc_str(value);
    if (list->head) {
        ll_node_add_str(list->head, node);
    } else {
        list->head = node;
    }
}

void ll_push_front_str(ll_str_t *list, char* value)
{
    list->count++;
    ll_node_str_t *node = ll_node_alloc_str(value);
    if (list->head) {
        list->head->prev = node;
        node->next = list->head;
    }
    list->head = node;
}

char* ll_at_str(ll_str_t *list, size_t index)
{
    assert(index < list->count);
    return ll_node_at_str(list->head, index)->value;
}

char* ll_remove_str(ll_str_t *list, size_t index)
{
    assert(list->count > 0);
    list->count--;
    ll_node_str_t *node = ll_node_at_str(list->head, index);
    return ll_node_remove_str(node);
}

size_t ll_count_str(ll_str_t *list)
{
    return list->count;
}
