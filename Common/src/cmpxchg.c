#include <common/cmpxchg.h>

int cmpxchg(int* pointer, int old_value, int new_value)
{
    return __sync_bool_compare_and_swap(pointer, old_value, new_value);
}
