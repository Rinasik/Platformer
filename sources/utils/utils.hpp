#pragma once

#include <vector>
#include <cmath>

inline double multiply(double x, double y)
{
    return ((round(x * 1000) * round(y * 1000)) / 1000000);
}

inline double add(double x, double y)
{
    return ((round(x * 1000) + round(y * 1000)) / 1000);
}

inline double decimalPart(double x)
{
    auto t = modf(x, new double);

    if (t < EPSILON)
    {
        return 0;
    }
    if (t >= 1 - EPSILON)
    {
        return 1;
    }
    return t;
}

inline int Sign(double x)
{
    if (x > 0)
    {
        return 1;
    }
    if (x < 0)
    {
        return -1;
    }
    return 0;
}

