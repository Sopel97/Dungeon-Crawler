#include "..\include\PerlinNoise.h"

#include "..\Util.h"

#include <cmath>
#include <limits>

namespace ls
{
    template <class T, int Dim, class Hasher>
    PerlinNoise<T, Dim, Hasher>::PerlinNoise(Hasher hasher) :
        m_hasher(std::move(hasher)),
        Super()
    {
        Super::removePeriod();
    }

    template <class T, int Dim, class Hasher>
    T PerlinNoise<T, Dim, Hasher>::operator()(const VectorType& coords) const
    {
        T raw =
            (Super::m_octaves > 1)
            ? fractalNoise(coords)
            : rawNoise(coords * Super::m_scale, Super::m_period, m_hasher);

        return detail::scaleResult(raw, Super::m_lowerBound, Super::m_upperBound);
    }

    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T PerlinNoise<T, Dim, Hasher>::rawNoise(T x, int px, const Hasher& hasher)
    {
        int ix0 = Util::fastFloor(x); // Integer part of x
        T fx0 = x - ix0;       // Fractional part of x
        T fx1 = fx0 - T(1);

        uint32_t ix0p0 = Util::applyPeriod(ix0, px);
        uint32_t ix0p1 = Util::applyPeriod(ix0 + 1, px);

        T s = detail::fade(fx0);

        T n0 = grad1(hasher(ix0p0), fx0);
        T n1 = grad1(hasher(ix0p1), fx1);

        return T(0.188) * (detail::lerp(n0, n1, s));
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T PerlinNoise<T, Dim, Hasher>::rawNoise(const Vec2<T>& coords, const Vec2<int>& period, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        int px = period.x;
        int py = period.y;

        int ix0 = Util::fastFloor(x); // Integer part of x
        int iy0 = Util::fastFloor(y); // Integer part of y
        T fx0 = x - ix0;        // Fractional part of x
        T fy0 = y - iy0;        // Fractional part of y
        T fx1 = fx0 - T(1);
        T fy1 = fy0 - T(1);

        uint32_t ix0p0 = Util::applyPeriod(ix0, px);
        uint32_t iy0p0 = Util::applyPeriod(iy0, py);
        uint32_t ix0p1 = Util::applyPeriod(ix0 + 1, px);
        uint32_t iy0p1 = Util::applyPeriod(iy0 + 1, py);

        T t = detail::fade(fy0);
        T s = detail::fade(fx0);

        T nx0 = grad2(hasher(ix0p0 + hasher(iy0p0)), fx0, fy0);
        T nx1 = grad2(hasher(ix0p0 + hasher(iy0p1)), fx0, fy1);
        T n0 = detail::lerp(nx0, nx1, t);

        nx0 = grad2(hasher(ix0p1 + hasher(iy0p0)), fx1, fy0);
        nx1 = grad2(hasher(ix0p1 + hasher(iy0p1)), fx1, fy1);
        T n1 = detail::lerp(nx0, nx1, t);

        return T(0.507) * (detail::lerp(n0, n1, s));
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T PerlinNoise<T, Dim, Hasher>::rawNoise(const Vec3<T>& coords, const Vec3<int>& period, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        T z = coords.z;
        int px = period.x;
        int py = period.y;
        int pz = period.z;

        int ix0 = Util::fastFloor(x); // Integer part of x
        int iy0 = Util::fastFloor(y); // Integer part of y
        int iz0 = Util::fastFloor(z); // Integer part of z
        T fx0 = x - ix0;        // Fractional part of x
        T fy0 = y - iy0;        // Fractional part of y
        T fz0 = z - iz0;        // Fractional part of z
        T fx1 = fx0 - T(1);
        T fy1 = fy0 - T(1);
        T fz1 = fz0 - T(1);

        uint32_t ix0p0 = Util::applyPeriod(ix0, px);
        uint32_t iy0p0 = Util::applyPeriod(iy0, py);
        uint32_t iz0p0 = Util::applyPeriod(iz0, pz);
        uint32_t ix0p1 = Util::applyPeriod(ix0 + 1, px);
        uint32_t iy0p1 = Util::applyPeriod(iy0 + 1, py);
        uint32_t iz0p1 = Util::applyPeriod(iz0 + 1, pz);

        T r = detail::fade(fz0);
        T t = detail::fade(fy0);
        T s = detail::fade(fx0);

        T nxy0 = grad3(hasher(ix0p0 + hasher(iy0p0 + hasher(iz0p0))), fx0, fy0, fz0);
        T nxy1 = grad3(hasher(ix0p0 + hasher(iy0p0 + hasher(iz0p1))), fx0, fy0, fz1);
        T nx0 = detail::lerp(nxy0, nxy1, r);

        nxy0 = grad3(hasher(ix0p0 + hasher(iy0p1 + hasher(iz0p0))), fx0, fy1, fz0);
        nxy1 = grad3(hasher(ix0p0 + hasher(iy0p1 + hasher(iz0p1))), fx0, fy1, fz1);
        T nx1 = detail::lerp(nxy0, nxy1, r);

        T n0 = detail::lerp(nx0, nx1, t);

        nxy0 = grad3(hasher(ix0p1 + hasher(iy0p0 + hasher(iz0p0))), fx1, fy0, fz0);
        nxy1 = grad3(hasher(ix0p1 + hasher(iy0p0 + hasher(iz0p1))), fx1, fy0, fz1);
        nx0 = detail::lerp(nxy0, nxy1, r);

        nxy0 = grad3(hasher(ix0p1 + hasher(iy0p1 + hasher(iz0p0))), fx1, fy1, fz0);
        nxy1 = grad3(hasher(ix0p1 + hasher(iy0p1 + hasher(iz0p1))), fx1, fy1, fz1);
        nx1 = detail::lerp(nxy0, nxy1, r);

        T n1 = detail::lerp(nx0, nx1, t);

        return T(0.936) * (detail::lerp(n0, n1, s));
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T PerlinNoise<T, Dim, Hasher>::rawNoise(const Vec4<T>& coords, const Vec4<int>& period, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        T z = coords.z;
        T w = coords.w;
        int px = period.x;
        int py = period.y;
        int pz = period.z;
        int pw = period.w;

        int ix0 = Util::fastFloor(x); // Integer part of x
        int iy0 = Util::fastFloor(y); // Integer part of y
        int iz0 = Util::fastFloor(z); // Integer part of y
        int iw0 = Util::fastFloor(w); // Integer part of w
        T fx0 = x - ix0;        // Fractional part of x
        T fy0 = y - iy0;        // Fractional part of y
        T fz0 = z - iz0;        // Fractional part of z
        T fw0 = w - iw0;        // Fractional part of w
        T fx1 = fx0 - T(1);
        T fy1 = fy0 - T(1);
        T fz1 = fz0 - T(1);
        T fw1 = fw0 - T(1);

        uint32_t ix0p0 = Util::applyPeriod(ix0, px);
        uint32_t iy0p0 = Util::applyPeriod(iy0, py);
        uint32_t iz0p0 = Util::applyPeriod(iz0, pz);
        uint32_t iw0p0 = Util::applyPeriod(iw0, pw);
        uint32_t ix0p1 = Util::applyPeriod(ix0 + 1, px);
        uint32_t iy0p1 = Util::applyPeriod(iy0 + 1, py);
        uint32_t iz0p1 = Util::applyPeriod(iz0 + 1, pz);
        uint32_t iw0p1 = Util::applyPeriod(iw0 + 1, pw);

        T q = detail::fade(fw0);
        T r = detail::fade(fz0);
        T t = detail::fade(fy0);
        T s = detail::fade(fx0);

        T nxyz0 = grad4(hasher(ix0p0 + hasher(iy0p0 + hasher(iz0p0 + hasher(iw0p0)))), fx0, fy0, fz0, fw0);
        T nxyz1 = grad4(hasher(ix0p0 + hasher(iy0p0 + hasher(iz0p0 + hasher(iw0p1)))), fx0, fy0, fz0, fw1);
        T nxy0 = detail::lerp(nxyz0, nxyz1, q);

        nxyz0 = grad4(hasher(ix0p0 + hasher(iy0p0 + hasher(iz0p1 + hasher(iw0p0)))), fx0, fy0, fz1, fw0);
        nxyz1 = grad4(hasher(ix0p0 + hasher(iy0p0 + hasher(iz0p1 + hasher(iw0p1)))), fx0, fy0, fz1, fw1);
        T nxy1 = detail::lerp(nxyz0, nxyz1, q);

        T nx0 = detail::lerp(nxy0, nxy1, r);

        nxyz0 = grad4(hasher(ix0p0 + hasher(iy0p1 + hasher(iz0p0 + hasher(iw0p0)))), fx0, fy1, fz0, fw0);
        nxyz1 = grad4(hasher(ix0p0 + hasher(iy0p1 + hasher(iz0p0 + hasher(iw0p1)))), fx0, fy1, fz0, fw1);
        nxy0 = detail::lerp(nxyz0, nxyz1, q);

        nxyz0 = grad4(hasher(ix0p0 + hasher(iy0p1 + hasher(iz0p1 + hasher(iw0p0)))), fx0, fy1, fz1, fw0);
        nxyz1 = grad4(hasher(ix0p0 + hasher(iy0p1 + hasher(iz0p1 + hasher(iw0p1)))), fx0, fy1, fz1, fw1);
        nxy1 = detail::lerp(nxyz0, nxyz1, q);

        T nx1 = detail::lerp(nxy0, nxy1, r);

        T n0 = detail::lerp(nx0, nx1, t);

        nxyz0 = grad4(hasher(ix0p1 + hasher(iy0p0 + hasher(iz0p0 + hasher(iw0p0)))), fx1, fy0, fz0, fw0);
        nxyz1 = grad4(hasher(ix0p1 + hasher(iy0p0 + hasher(iz0p0 + hasher(iw0p1)))), fx1, fy0, fz0, fw1);
        nxy0 = detail::lerp(nxyz0, nxyz1, q);

        nxyz0 = grad4(hasher(ix0p1 + hasher(iy0p0 + hasher(iz0p1 + hasher(iw0p0)))), fx1, fy0, fz1, fw0);
        nxyz1 = grad4(hasher(ix0p1 + hasher(iy0p0 + hasher(iz0p1 + hasher(iw0p1)))), fx1, fy0, fz1, fw1);
        nxy1 = detail::lerp(nxyz0, nxyz1, q);

        nx0 = detail::lerp(nxy0, nxy1, r);

        nxyz0 = grad4(hasher(ix0p1 + hasher(iy0p1 + hasher(iz0p0 + hasher(iw0p0)))), fx1, fy1, fz0, fw0);
        nxyz1 = grad4(hasher(ix0p1 + hasher(iy0p1 + hasher(iz0p0 + hasher(iw0p1)))), fx1, fy1, fz0, fw1);
        nxy0 = detail::lerp(nxyz0, nxyz1, q);

        nxyz0 = grad4(hasher(ix0p1 + hasher(iy0p1 + hasher(iz0p1 + hasher(iw0p0)))), fx1, fy1, fz1, fw0);
        nxyz1 = grad4(hasher(ix0p1 + hasher(iy0p1 + hasher(iz0p1 + hasher(iw0p1)))), fx1, fy1, fz1, fw1);
        nxy1 = detail::lerp(nxyz0, nxyz1, q);

        nx1 = detail::lerp(nxy0, nxy1, r);

        T n1 = detail::lerp(nx0, nx1, t);

        return T(0.87) * (detail::lerp(n0, n1, s));
    }


    template <class T, int Dim, class Hasher>
    T PerlinNoise<T, Dim, Hasher>::fractalNoise(const VectorType& coords) const
    {
        T total = T(0);
        VectorType frequency = Super::m_scale;
        VectorTypeI period = Super::m_period;

        T amplitude = T(1);
        // We have to keep track of the sum of the amplitudes,
        // because each octave adds more, and we need a value in [-1, 1].
        T amplitudeSum = T(0);

        for(int i = 0; i < Super::m_octaves; ++i)
        {
            total += rawNoise(coords * frequency, period, m_hasher) * amplitude;

            frequency *= T(2);
            Super::doublePeriod(period);
            amplitudeSum += amplitude;
            amplitude *= Super::m_persistence;
        }

        return total / amplitudeSum;
    }
}