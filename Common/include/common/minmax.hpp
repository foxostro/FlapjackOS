#pragma once

template <class T> const T& MIN(const T& a, const T& b)
{
    return !(b<a)?a:b;
}

template <class T> const T& MAX(const T& a, const T& b)
{
    return (a<b)?b:a;
}
