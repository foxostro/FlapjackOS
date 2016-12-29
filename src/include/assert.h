#pragma once

#include <stdlib.h>
#include <panic.h>

#ifdef TESTING
#define assert(expr) ((void)((expr) ? 0 : (abort(), 0)))
#else
#define assert(expr) ((void)((expr) ? 0 : (panic("%s:%u: failed assertion `%s'", __FILE__, __LINE__, # expr), 0)))
#endif
