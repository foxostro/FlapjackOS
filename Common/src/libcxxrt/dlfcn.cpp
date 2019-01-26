#include "dlfcn.h"
#include <cstddef>

extern "C"
void *dlopen(__attribute__((unused)) const char *filename,
             __attribute__((unused)) int flags)
{
    return 0;
}

extern "C"
void *dlsym(__attribute__((unused)) void *handle,
            __attribute__((unused)) const char *name)
{
    return nullptr;
}

extern "C"
int dlclose(__attribute__((unused)) void *handle)
{
    return -1;
}

extern "C"
char *dlerror()
{
    return nullptr;
}

extern "C"
int dladdr(__attribute__((unused)) void *addr,
           __attribute__((unused)) Dl_info *info)
{
    return 0;
}
