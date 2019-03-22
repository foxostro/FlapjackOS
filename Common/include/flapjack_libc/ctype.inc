static inline bool isprint(int c)
{
    return (c >= ' ') && (c <= 126);
}

static inline int isdigit(int c)
{
    return (c >= '0') && (c <= '9');
}

static inline int islower(int c)
{
    return (c >= 'a') && (c <= 'z');
}

static inline int isupper(int c)
{
    return (c >= 'A') && (c <= 'Z');
}
