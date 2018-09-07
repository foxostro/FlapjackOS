#ifndef FLAPJACKOS_COMMON_INCLUDE_COMMON_MINMAX_HPP
#define FLAPJACKOS_COMMON_INCLUDE_COMMON_MINMAX_HPP

template <class T> const T& MIN(const T& a, const T& b)
{
    return !(b<a)?a:b;
}

template <class T> const T& MAX(const T& a, const T& b)
{
    return (a<b)?b:a;
}

#endif // FLAPJACKOS_COMMON_INCLUDE_COMMON_MINMAX_HPP
