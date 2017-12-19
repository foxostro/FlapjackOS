#pragma once

#ifdef __cplusplus
extern "C" {
#endif

static inline int isprint(int c)
{
    return ((c) >= ' ') && ((c) <= 126);
}

#ifdef __cplusplus
}
#endif