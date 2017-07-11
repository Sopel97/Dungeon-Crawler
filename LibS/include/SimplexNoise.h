#pragma once

#include "..\Fwd.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "detail/NoiseDetail.h"

// Uses slightly modified implementation by Stefan Gustavson (stegu@itn.liu.se) (2003-2005)
// Original code can be found here https://github.com/kev009/craftd/tree/master/plugins/survival/mapgen/noise

namespace ls
{
    struct ComputeDerivativeTag {};

    template <class T, int Dim, class Hasher>
    class SimplexNoise : public detail::PerlinSimplexInterface<T, Dim>
    {
        static_assert(Dim >= 1 && Dim <= 4, "Dim must be in range 1-4");
        using Super = detail::PerlinSimplexInterface<T, Dim>;
    public:
        using ValueType = T;
        using VectorType = typename Util::VectorType<T, Dim>::type;
        using VectorTypeI = typename Util::VectorType<int, Dim>::type;

        SimplexNoise(Hasher hasher = Hasher());

        T operator()(const VectorType& coords) const;

        template <int D = Dim, class SFINAE = typename std::enable_if<D == 1, T>::type>
        static T rawNoise(T x, int px, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 1, T>::type>
        static detail::ValueDerivativePair<T, 1> rawNoise(T x, int px, ComputeDerivativeTag, const Hasher& hasher = Hasher());

        template <int D = Dim, class SFINAE = typename std::enable_if<D == 2, T>::type>
        static T rawNoise(const Vec2<T>& coords, const Vec2I& period, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 2, T>::type>
        static detail::ValueDerivativePair<T, 2> rawNoise(const Vec2<T>& coords, const Vec2I& period, ComputeDerivativeTag, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 2, T>::type>
        static detail::ValueDerivativePair<T, 2> rawNoise(const Vec2<T>& coords, const Vec2I& period, T angle, const Hasher& hasher = Hasher());

        template <int D = Dim, class SFINAE = typename std::enable_if<D == 3, T>::type>
        static T rawNoise(const Vec3<T>& coords, const Vec3I& period, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 3, T>::type>
        static detail::ValueDerivativePair<T, 3> rawNoise(const Vec3<T>& coords, const Vec3I& period, ComputeDerivativeTag, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 3, T>::type>
        static detail::ValueDerivativePair<T, 3> rawNoise(const Vec3<T>& coords, const Vec3I& period, T angle, const Hasher& hasher = Hasher());

        template <int D = Dim, class SFINAE = typename std::enable_if<D == 4, T>::type>
        static T rawNoise(const Vec4<T>& coords, const Vec4I& period, const Hasher& hasher = Hasher());
        template <int D = Dim, class SFINAE = typename std::enable_if<D == 4, T>::type>
        static detail::ValueDerivativePair<T, 4> rawNoise(const Vec4<T>& coords, const Vec4I& period, ComputeDerivativeTag, const Hasher& hasher = Hasher());

    private:
        static T grad1(uint32_t hash, T x)
        {
            uint32_t h = hash & 0b1111u;
            T grad = T(1) + (h & 0b111u);   // Gradient value 1.0, 2.0, ..., 8.0
            if(h & 0b1000u) grad = -grad;   // Set a random sign for the gradient
            return (grad * x);              // Multiply the gradient with the distance
        }
        static void grad1d(uint32_t hash, T& gx)
        {
            gx = grad1(hash, T(1));
        }

        static T grad2(uint32_t hash, T x, T y)
        {
            uint32_t h = hash & 0b111u;      // Convert low 3 bits of hash code
            T u = h < 4 ? x : y;  // into 8 simple gradient directions,
            T v = h < 4 ? y : x;  // and compute the dot product with (x,y).
            return ((h & 0b001u) ? -u : u) + ((h & 0b010u) ? T(-2)*v : T(2)*v);
        }
        static void grad2d(uint32_t hash, T& gx, T& gy)
        {
            uint32_t h = hash & 0b111u;
            gx = m_grad2lut[h][0];
            gy = m_grad2lut[h][1];
        }
        static T graddotp2(T gx, T gy, T x, T y)
        {
            return gx * x + gy * y;
        }
        static void gradrot2(uint32_t hash, T sin_t, T cos_t, T& gx, T& gy)
        {
            uint32_t h = hash & 0b111u;
            T gx0 = m_grad2lut[h][0];
            T gy0 = m_grad2lut[h][1];
            gx = cos_t * gx0 - sin_t * gy0;
            gy = sin_t * gx0 + cos_t * gy0;
        }
        
        static constexpr T m_F2 = T(0.366025403); // F2 = (sqrt(3)-1)/2 //TODO: make these static members
        static constexpr T m_G2 = T(0.211324865); // G2 = (3-sqrt(3))/6

                                                  /*
                                                  * Gradient tables. These could be programmed the Ken Perlin way with
                                                  * some clever bit-twiddling, but this is more clear, and not really slower.
                                                  */
        static constexpr T m_grad2lut[8][2] = {
            {-1.0, -1.0},{1.0, 0.0},{-1.0, 0.0},{1.0, 1.0},
            {-1.0, 1.0},{0.0, -1.0},{0.0, 1.0},{1.0, -1.0}
        };

        static T grad3(uint32_t hash, T x, T y, T z)
        {
            uint32_t h = hash & 0b1111u;     // Convert low 4 bits of hash code into 12 simple
            T u = h < 8 ? x : y; // gradient directions, and compute dot product.
            T v = h < 4 ? y : h == 12 || h == 14 ? x : z; // Fix repeats at h = 12 to 15
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? -v : v);
        }
        static void grad3d(uint32_t hash, T& gx, T& gy, T& gz)
        {
            uint32_t h = hash & 0b1111u;
            gx = m_grad3lut[h][0];
            gy = m_grad3lut[h][1];
            gz = m_grad3lut[h][2];
        }
        static T graddotp3(T gx, T gy, T gz, T x, T y, T z)
        {
            return gx * x + gy * y + gz * z;
        }
        static void gradrot3(uint32_t hash, T sin_t, T cos_t, T& gx, T& gy, T& gz)
        {
            uint32_t h = hash & 0b1111;
            T gux = m_grad3u[h][0];
            T guy = m_grad3u[h][1];
            T guz = m_grad3u[h][2];
            T gvx = m_grad3v[h][0];
            T gvy = m_grad3v[h][1];
            T gvz = m_grad3v[h][2];
            gx = cos_t * gux + sin_t * gvx;
            gy = cos_t * guy + sin_t * gvy;
            gz = cos_t * guz + sin_t * gvz;
        }

        // Simple skewing factors for the 3D case
        static constexpr T m_F3 = T(1.0 / 3.0);
        static constexpr T m_G3 = T(1.0 / 6.0);

        /*
        * Gradient directions for 3D.
        * These vectors are based on the midpoints of the 12 edges of a cube.
        * A larger array of random unit length vectors would also do the job,
        * but these 12 (including 4 repeats to make the array length a power
        * of two) work better. They are not random, they are carefully chosen
        * to represent a small, isotropic set of directions.
        */
        static constexpr T m_grad3lut[16][3] = {
            {1.0, 0.0, 1.0},{0.0, 1.0, 1.0}, // 12 cube edges
            {-1.0, 0.0, 1.0},{0.0, -1.0, 1.0},
            {1.0, 0.0, -1.0},{0.0, 1.0, -1.0},
            {-1.0, 0.0, -1.0},{0.0, -1.0, -1.0},
            {1.0, -1.0, 0.0},{1.0, 1.0, 0.0},
            {-1.0, 1.0, 0.0},{-1.0, -1.0, 0.0},
            {1.0, 0.0, 1.0},{-1.0, 0.0, 1.0}, // 4 repeats to make 16
            {0.0, 1.0, -1.0},{0.0, -1.0, -1.0}
        };

        /*
        * For 3D, we define two orthogonal vectors in the desired rotation plane.
        * These vectors are based on the midpoints of the 12 edges of a cube,
        * they all rotate in their own plane and are never coincident or collinear.
        * A larger array of random vectors would also do the job, but these 12
        * (including 4 repeats to make the array length a power of two) work better.
        * They are not random, they are carefully chosen to represent a small
        * isotropic set of directions for any rotation angle.
        */
        /* a = sqrt(2)/sqrt(3) = 0.816496580 */
        static constexpr T m_a = T(0.81649658);
        static constexpr T m_grad3u[16][3] = {
            {1.0f, 0.0f, 1.0f},{0.0f, 1.0f, 1.0f}, // 12 cube edges
            {-1.0f, 0.0f, 1.0f},{0.0f, -1.0f, 1.0f},
            {1.0f, 0.0f, -1.0f},{0.0f, 1.0f, -1.0f},
            {-1.0f, 0.0f, -1.0f},{0.0f, -1.0f, -1.0f},
            {m_a, m_a, m_a},{-m_a, m_a, -m_a},
            {-m_a, -m_a, m_a},{m_a, -m_a, -m_a},
            {-m_a, m_a, m_a},{m_a, -m_a, m_a},
            {m_a, -m_a, -m_a},{-m_a, m_a, -m_a}
        };

        static constexpr T m_grad3v[16][3] = {
            {-m_a, m_a, m_a},{-m_a, -m_a, m_a},
            {m_a, -m_a, m_a},{m_a, m_a, m_a},
            {-m_a, -m_a, -m_a},{m_a, -m_a, -m_a},
            {m_a, m_a, -m_a},{-m_a, m_a, -m_a},
            {1.0f, -1.0f, 0.0f},{1.0f, 1.0f, 0.0f},
            {-1.0f, 1.0f, 0.0f},{-1.0f, -1.0f, 0.0f},
            {1.0f, 0.0f, 1.0f},{-1.0f, 0.0f, 1.0f}, // 4 repeats to make 16
            {0.0f, 1.0f, -1.0f},{0.0f, -1.0f, -1.0f}
        };

        static T grad4(uint32_t hash, T x, T y, T z, T t)
        {
            uint32_t h = hash & 0b11111u;      // Convert low 5 bits of hash code into 32 simple
            T u = h < 24 ? x : y; // gradient directions, and compute dot product.
            T v = h < 16 ? y : z;
            T w = h < 8 ? z : t;
            return ((h & 0b1u) ? -u : u) + ((h & 0b10u) ? -v : v) + ((h & 0b100u) ? -w : w);
        }
        static void grad4d(uint32_t hash, T& gx, T& gy, T& gz, T& gw)
        {
            uint32_t h = hash & 0b11111u;
            gx = m_grad4lut[h][0];
            gy = m_grad4lut[h][1];
            gz = m_grad4lut[h][2];
            gw = m_grad4lut[h][3];
        }
        static T graddotp4(T gx, T gy, T gz, T gw, T x, T y, T z, T w)
        {
            return gx * x + gy * y + gz * z + gw * w;
        }

        // The skewing and unskewing factors are hairy again for the 4D case
        static constexpr T m_F4 = T(0.309016994); // F4 = (sqrt(5)-1)/4
        static constexpr T m_G4 = T(0.138196601); // G4 = (5-sqrt(5))/20

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
        static constexpr T m_grad4lut[32][4] = {
            {0.0, 1.0, 1.0, 1.0},{0.0, 1.0, 1.0, -1.0},{0.0, 1.0, -1.0, 1.0},{0.0, 1.0, -1.0, -1.0}, // 32 tesseract edges
            {0.0, -1.0, 1.0, 1.0},{0.0, -1.0, 1.0, -1.0},{0.0, -1.0, -1.0, 1.0},{0.0, -1.0, -1.0, -1.0},
            {1.0, 0.0, 1.0, 1.0},{1.0, 0.0, 1.0, -1.0},{1.0, 0.0, -1.0, 1.0},{1.0, 0.0, -1.0, -1.0},
            {-1.0, 0.0, 1.0, 1.0},{-1.0, 0.0, 1.0, -1.0},{-1.0, 0.0, -1.0, 1.0},{-1.0, 0.0, -1.0, -1.0},
            {1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 0.0, -1.0},{1.0, -1.0, 0.0, 1.0},{1.0, -1.0, 0.0, -1.0},
            {-1.0, 1.0, 0.0, 1.0},{-1.0, 1.0, 0.0, -1.0},{-1.0, -1.0, 0.0, 1.0},{-1.0, -1.0, 0.0, -1.0},
            {1.0, 1.0, 1.0, 0.0},{1.0, 1.0, -1.0, 0.0},{1.0, -1.0, 1.0, 0.0},{1.0, -1.0, -1.0, 0.0},
            {-1.0, 1.0, 1.0, 0.0},{-1.0, 1.0, -1.0, 0.0},{-1.0, -1.0, 1.0, 0.0},{-1.0, -1.0, -1.0, 0.0}
        };

        template <class Ret, class... Args>
        Ret fractal(const VectorType& coords, Args... args) const;
        
        Hasher m_hasher;
    };

    using SimplexNoise1D = SimplexNoise<double, 1>;
    using SimplexNoise1F = SimplexNoise<float, 1>;

    using SimplexNoise2D = SimplexNoise<double, 2>;
    using SimplexNoise2F = SimplexNoise<float, 2>;

    using SimplexNoise3D = SimplexNoise<double, 3>;
    using SimplexNoise3F = SimplexNoise<float, 3>;

    using SimplexNoise4D = SimplexNoise<double, 4>;
    using SimplexNoise4F = SimplexNoise<float, 4>;
}
#include "../src/SimplexNoise.cpp"
