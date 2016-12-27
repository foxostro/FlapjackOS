#pragma once

#include <panic.h>

#define assert(expr) ((void)((expr) ? 0 : (panic("%s:%u: failed assertion `%s'", __FILE__, __LINE__, # expr), 0)))
