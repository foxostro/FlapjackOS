// ll_str.h is an automatically generated file. Do not edit directly.




#include <stddef.h>

// A node in a linked list which contains items with type "char*".
typedef struct ll_node_str {
    struct ll_node_str *prev, *next;
    char* value;
} ll_node_str_t;

// Allocate a new node with the specified value.
ll_node_str_t * ll_node_alloc(char* value);

// Add the specified node to the end of the specified list.
void ll_node_add_str(ll_node_str_t *head, ll_node_str_t *node);

// Gets the index'th node after `head'.
// Returns the head if index is zero.
ll_node_str_t * ll_node_at_str(ll_node_str_t *head, size_t index);

// Remove the specified node from the list.
// Returns the value of the removed item.
char* ll_node_remove_str(ll_node_str_t *node);


// Represents the entire linked list, which is slightly more convenient than
// passing around the head node everywhere.
typedef struct ll_str {
    ll_node_str_t *head;
    size_t count;
} ll_str_t;

// Create a new, empty linked list.
ll_str_t * ll_init_str(void);

// Destroy an existing linked list.
void ll_destroy_str(ll_str_t *list);

// Inserts an item at the tail of the linked list.
void ll_push_back_str(ll_str_t *list, char* value);

// Inserts an item at the head of the linked list.
void ll_push_front_str(ll_str_t *list, char* value);

// Gets the index'th value of the list.
// Returns the value of the head if index is zero.
char* ll_at_str(ll_str_t *list, size_t index);

// Removes the index'th item in the list.
// Returns the value of the removed item.
char* ll_remove_str(ll_str_t *list, size_t index);

// Returns the number of items in the list.
size_t ll_count_str(ll_str_t *list);
