#include <common/spin_lock.h>
#include <common/cmpxchg.h>

static void memory_barrier(void)
{
    asm volatile("":::"memory");
}

static void pause(void)
{
    asm volatile("pause":::);
}

void spin_lock(int* lock)
{
    while(!cmpxchg(lock, 0, 1)) {
        while(*lock == 0) {
            pause();
        }
    }
}

void spin_unlock(int volatile* lock)
{
    memory_barrier();
    *lock = 0;
}
