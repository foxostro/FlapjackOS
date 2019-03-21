#include <common/mutex.hpp>

static void nop() {}

void (*Mutex::yield)() = ::nop;
