#pragma once

static inline int isprint(int c)
{
    return ((c) >= ' ') && ((c) <= 126);
}