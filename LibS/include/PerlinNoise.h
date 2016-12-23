#pragma once

#include <numeric>
#include <algorithm>

#include "../Fwd.h"
#include "detail/NoiseDetail.h"
#include "../Util.h"

// Uses slightly modified implementation by Stefan Gustavson (stegu@itn.liu.se) (2003-2005)
// Original code can be found on https://github.com/kev009/craftd/tree/master/plugins/survival/mapgen/noise

namespace ls
{
    template <class T, int Dim, class Hasher>
    class PerlinNoise : public detail::PerlinSimplexInterface<T, Dim>
    {
        static_assert(Dim >= 1 && Dim <= 4, "Dim must be in range 1-4");
        using Super = detail::PerlinSimplexInterface<T, Dim>;
    public:
        using ValueType = T;
        using VectorType = typename Util::VectorType<T, Dim>::type;
        using VectorTypeI = typename Util::VectorType<int, Dim>::type;

        PerlinNoise(Hasher hasher = Hasher());

        T operator()(const VectorType& coords) const;

        template <int D = Dim, class SFINAE = typename std::enable_if<D == 1, T>::type>
        static T rawNoise(T x, int px, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 2, T>::type>
        static T rawNoise(const Vec2<T>& coords, const Vec2<int>& period, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 3, T>::type>
        static T rawNoise(const Vec3<T>& coords, const Vec3<int>& period, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 4, T>::type>
        static T rawNoise(const Vec4<T>& coords, const Vec4<int>& period, const Hasher& hasher = Hasher());

    private:
        static T grad1(uint32_t hash, T x)
        {
            uint32_t h = hash & 0b1111u;
            T grad = T(1) + (h & 0b111u);   // Gradient value 1.0, 2.0, ..., 8.0
            if(h & 0b1000u) grad = -grad;   // Set a random sign for the gradient
            return (grad * x);              // Multiply the gradient with the distance
        }
        static T grad2(uint32_t hash, T x, T y)
        {
            uint32_t h = hash & 0b111u;      // Convert low 3 bits of hash code
            T u = h < 4 ? x : y;  // into 8 simple gradient directions,
            T v = h < 4 ? y : x;  // and compute the dot product with (x,y).
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? T(-2)*v : T(2)*v);
        }
        static T grad3(uint32_t hash, T x, T y, T z)
        {
            uint32_t h = hash & 0b1111u;     // Convert low 4 bits of hash code into 12 simple
            T u = h < 8 ? x : y; // gradient directions, and compute dot product.
            T v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? -v : v);
        }
        static T grad4(uint32_t hash, T x, T y, T z, T t)
        {
            uint32_t h = hash & 0b11111u;      // Convert low 5 bits of hash code into 32 simple
            T u = h < 24 ? x : y; // gradient directions, and compute dot product.
            T v = h < 16 ? y : z;
            T w = h < 8 ? z : t;
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? -v : v) + ((h & 0b100u) ? -w : w);
        }
        static constexpr uint8_t m_simplex[64][4] = {
            {0, 1, 2, 3},{0, 1, 3, 2},{0, 0, 0, 0},{0, 2, 3, 1},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{1, 2, 3, 0},
            {0, 2, 1, 3},{0, 0, 0, 0},{0, 3, 1, 2},{0, 3, 2, 1},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{1, 3, 2, 0},
            {0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},
            {1, 2, 0, 3},{0, 0, 0, 0},{1, 3, 0, 2},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{2, 3, 0, 1},{2, 3, 1, 0},
            {1, 0, 2, 3},{1, 0, 3, 2},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{2, 0, 3, 1},{0, 0, 0, 0},{2, 1, 3, 0},
            {0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},
            {2, 0, 1, 3},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{3, 0, 1, 2},{3, 0, 2, 1},{0, 0, 0, 0},{3, 1, 2, 0},
            {2, 1, 0, 3},{0, 0, 0, 0},{0, 0, 0, 0},{0, 0, 0, 0},{3, 1, 0, 2},{0, 0, 0, 0},{3, 2, 0, 1},{3, 2, 1, 0}
        };

        T fractalNoise(const VectorType& coords) const;

        Hasher m_hasher;
    };

    using PerlinNoise1D = PerlinNoise<double, 1>;
    using PerlinNoise1F = PerlinNoise<float, 1>;

    using PerlinNoise2D = PerlinNoise<double, 2>;
    using PerlinNoise2F = PerlinNoise<float, 2>;

    using PerlinNoise3D = PerlinNoise<double, 3>;
    using PerlinNoise3F = PerlinNoise<float, 3>;

    using PerlinNoise4D = PerlinNoise<double, 4>;
    using PerlinNoise4F = PerlinNoise<float, 4>;


    extern template class PerlinNoise<double, 1>;
    extern template class PerlinNoise<float, 1>;

    extern template class PerlinNoise<double, 2>;
    extern template class PerlinNoise<float, 2>;

    extern template class PerlinNoise<double, 3>;
    extern template class PerlinNoise<float, 3>;

    extern template class PerlinNoise<double, 4>;
    extern template class PerlinNoise<float, 4>;
}
#include "../src/PerlinNoise.cpp"
