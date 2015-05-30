#ifndef NOISE_H_INCLUDED
#define NOISE_H_INCLUDED

#include <cmath>
#include <random>
#include <algorithm>
#include <functional>
namespace Noise
{
    const double PI = 3.14159265358979323846;
    const double INV_PI = 0.31830988618379067154;
    template<class T>
    T max(const T& a, const T& b)
    {
        if(a > b) return a;
        return b;
    }
    template <class T>
    int fastFloor(const T x)
    {
        return x > 0 ? (int) x : (int) x - 1;
    }

    template <class T> class SimplexNoise;
    template <class T> class CellNoise;

#include "include/SimplexNoise.h"
#include "include/CellNoise.h"
}

#endif // NOISE_H_INCLUDED
