extern "C" {
#include "sched.h"
}

#include <common/mutex.hpp>

extern "C" int sched_yield()
{
    Mutex::yield();
    return 0;
}
