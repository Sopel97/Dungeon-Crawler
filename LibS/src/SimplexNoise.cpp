#include "..\include\SimplexNoise.h"

#include "..\Util.h"

#include <cmath>
#include <limits>

namespace ls
{
    template <class T, int Dim, class Hasher>
    SimplexNoise<T, Dim, Hasher>::SimplexNoise(Hasher hasher) :
        m_hasher(std::move(hasher)),
        Super()
    {
        Super::removePeriod();
    }

    template <class T, int Dim, class Hasher>
    T SimplexNoise<T, Dim, Hasher>::operator()(const VectorType& coords) const
    {
        T raw =
            (Super::m_octaves > 1)
            ? fractal<T>(coords)
            : rawNoise(coords * Super::m_scale, Super::m_period, m_hasher);

        return detail::scaleResult(raw, Super::m_lowerBound, Super::m_upperBound);
    }

    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T SimplexNoise<T, Dim, Hasher>::rawNoise(T x, int px, const Hasher& hasher)
    {
        int i0 = Util::fastFloor(x);
        T x0 = x - i0;
        T x1 = x0 - T(1);

        uint32_t i0p0 = Util::applyPeriod(i0, px);
        uint32_t i0p1 = Util::applyPeriod(i0 + 1, px);

        T n0, n1;

        T t0 = T(1) - x0*x0;
        //  if(t0 < 0.0f) t0 = 0.0f; // this never happens for the 1D case
        t0 *= t0;
        n0 = t0 * t0 * grad1(hasher(i0p0), x0);

        T t1 = T(1) - x1*x1;
        //  if(t1 < 0.0f) t1 = 0.0f; // this never happens for the 1D case
        t1 *= t1;
        n1 = t1 * t1 * grad1(hasher(i0p1), x1);
        // The maximum value of this noise is 8*(3/4)^4 = 2.53125
        // A factor of 0.395 scales to fit exactly within [-1,1], but
        // to match PRMan's 1D noise, one would use 0.25.
        return T(0.395) * (n0 + n1);
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    detail::ValueDerivativePair<T, 1> SimplexNoise<T, Dim, Hasher>::rawNoise(T x, int px, ComputeDerivativeTag, const Hasher& hasher)
    {
        int i0 = Util::fastFloor(x);
        T x0 = x - i0;
        T x1 = x0 - T(1);

        uint32_t i0p0 = Util::applyPeriod(i0, px);
        uint32_t i0p1 = Util::applyPeriod(i0 + 1, px);

        T gx0, gx1;
        T n0, n1;
        T t20, t40, t21, t41;

        T x20 = x0*x0;
        T t0 = T(1) - x20;
        //  if(t0 < 0.0f) t0 = 0.0f; // Never happens for 1D: x0<=1 always
        t20 = t0 * t0;
        t40 = t20 * t20;
        grad1d(hasher(i0p0), gx0);
        n0 = t40 * gx0 * x0;

        T x21 = x1*x1;
        T t1 = T(1) - x21;
        //  if(t1 < 0.0f) t1 = 0.0f; // Never happens for 1D: |x1|<=1 always
        t21 = t1 * t1;
        t41 = t21 * t21;
        grad1d(hasher(i0p1), gx1);
        n1 = t41 * gx1 * x1;

        T noise = T(0.395) * (n0 + n1);

        /* Compute derivative according to:
        *  T dx = -8.0f * t20 * t0 * x0 * (gx0 * x0) + t40 * gx0;
        *  dx += -8.0f * t21 * t1 * x1 * (gx1 * x1) + t41 * gx1;
        */
        T dx = t20 * t0 * gx0 * x20;
        dx += t21 * t1 * gx1 * x21;
        dx *= -T(-8);
        dx += t40 * gx0 + t41 * gx1;
        dx *= T(0.395); /* Scale derivative to match the noise scaling */

                        // The maximum value of this noise is 8*(3/4)^4 = 2.53125
                        // A factor of 0.395 would scale to fit exactly within [-1,1], but
                        // to better match classic Perlin noise, we scale it down some more (0.25).
        return detail::ValueDerivativePair<T, 1>(noise, dx);
    }

    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec2<T>& coords, const Vec2I& period, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        int px = period.x;
        int py = period.y;

        // Skew the input space to determine which simplex cell we're in
        T s = (x + y)*m_F2; // Hairy factor for 2D
        T xs = x + s;
        T ys = y + s;
        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);

        T t = T(i + j)*m_G2;
        T X0 = i - t; // Unskew the cell origin back to (x,y) space
        T Y0 = j - t;
        T x0 = x - X0; // The x,y distances from the cell origin
        T y0 = y - Y0;

        // For the 2D case, the simplex shape is an equilateral triangle.
        // Determine which simplex we are in.
        int i1, j1; // Offsets for second (middle) corner of simplex in (i,j) coords
        if(x0 > y0) { i1 = 1; j1 = 0; } // lower triangle, XY order: (0,0)->(1,0)->(1,1)
        else { i1 = 0; j1 = 1; }      // upper triangle, YX order: (0,0)->(0,1)->(1,1)

                                      // A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
                                      // a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
                                      // c = (3-sqrt(3))/6 = G2

        T x1 = x0 - i1 + m_G2; // Offsets for middle corner in (x,y) unskewed coords
        T y1 = y0 - j1 + m_G2;
        T x2 = x0 - T(1) + T(2) * m_G2; // Offsets for last corner in (x,y) unskewed coords
        T y2 = y0 - T(1) + T(2) * m_G2;

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t iip2 = Util::applyPeriod(i + 1, px);
        uint32_t jjp2 = Util::applyPeriod(j + 1, py);

        T n0, n1, n2; // Noise contributions from the three corners

                      // Calculate the contribution from the three corners
        T t0 = T(0.5) - x0*x0 - y0*y0;
        if(t0 < T(0)) n0 = T(0);
        else
        {
            t0 *= t0;
            n0 = t0 * t0 * grad2(hasher(iip0 + hasher(jjp0)), x0, y0);
        }

        T t1 = T(0.5) - x1*x1 - y1*y1;
        if(t1 < T(0)) n1 = T(0);
        else
        {
            t1 *= t1;
            n1 = t1 * t1 * grad2(hasher(iip1 + hasher(jjp1)), x1, y1);
        }

        T t2 = T(0.5) - x2*x2 - y2*y2;
        if(t2 < T(0)) n2 = T(0);
        else
        {
            t2 *= t2;
            n2 = t2 * t2 * grad2(hasher(iip2 + hasher(jjp2)), x2, y2);
        }

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to return values in the interval [-1,1].
        return T(40) * (n0 + n1 + n2); // TODO: The scale factor is preliminary! 
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    detail::ValueDerivativePair<T, 2> SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec2<T>& coords, const Vec2I& period, ComputeDerivativeTag, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        int px = period.x;
        int py = period.y;

        /* Skew the input space to determine which simplex cell we're in */
        T s = (x + y) * m_F2; /* Hairy factor for 2D */
        T xs = x + s;
        T ys = y + s;
        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);

        T t = T(i + j) * m_G2;
        T X0 = i - t; /* Unskew the cell origin back to (x,y) space */
        T Y0 = j - t;
        T x0 = x - X0; /* The x,y distances from the cell origin */
        T y0 = y - Y0;

        /* For the 2D case, the simplex shape is an equilateral triangle.
        * Determine which simplex we are in. */
        int i1, j1; /* Offsets for second (middle) corner of simplex in (i,j) coords */
        if(x0 > y0) { i1 = 1; j1 = 0; } /* lower triangle, XY order: (0,0)->(1,0)->(1,1) */
        else { i1 = 0; j1 = 1; }      /* upper triangle, YX order: (0,0)->(0,1)->(1,1) */

                                      /* A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
                                      * a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
                                      * c = (3-sqrt(3))/6   */
        T x1 = x0 - i1 + m_G2; /* Offsets for middle corner in (x,y) unskewed coords */
        T y1 = y0 - j1 + m_G2;
        T x2 = x0 - T(1) + T(2) * m_G2; /* Offsets for last corner in (x,y) unskewed coords */
        T y2 = y0 - T(1) + T(2) * m_G2;

        /* Wrap the integer indices at 256, to avoid indexing perm[] out of bounds */

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t iip2 = Util::applyPeriod(i + 1, px);
        uint32_t jjp2 = Util::applyPeriod(j + 1, py);

        /* Calculate the contribution from the three corners */
        T t0 = T(0.5) - x0 * x0 - y0 * y0;
        T t20, t40;
        T n0, n1, n2; /* Noise contributions from the three simplex corners */
        T gx0, gy0, gx1, gy1, gx2, gy2; /* Gradients at simplex corners */
        if(t0 < T(0)) t40 = t20 = t0 = n0 = gx0 = gy0 = T(0); /* No influence */
        else
        {
            grad2d(hasher(iip0 + hasher(jjp0)), gx0, gy0);
            t20 = t0 * t0;
            t40 = t20 * t20;
            n0 = t40 * (gx0 * x0 + gy0 * y0);
        }

        T t1 = T(0.5) - x1 * x1 - y1 * y1;
        T t21, t41;
        if(t1 < T(0)) t21 = t41 = t1 = n1 = gx1 = gy1 = T(0); /* No influence */
        else
        {
            grad2d(hasher(iip1 + hasher(jjp1)), gx1, gy1);
            t21 = t1 * t1;
            t41 = t21 * t21;
            n1 = t41 * (gx1 * x1 + gy1 * y1);
        }

        T t2 = T(0.5) - x2 * x2 - y2 * y2;
        T t22, t42;
        if(t2 < T(0)) t42 = t22 = t2 = n2 = gx2 = gy2 = T(0); /* No influence */
        else
        {
            grad2d(hasher(iip2 + hasher(jjp2)), gx2, gy2);
            t22 = t2 * t2;
            t42 = t22 * t22;
            n2 = t42 * (gx2 * x2 + gy2 * y2);
        }

        /* Add contributions from each corner to get the final noise value.
        * The result is scaled to return values in the interval [-1,1]. */
        T noise = T(40) * (n0 + n1 + n2);

        /* Compute derivative */
        /*  A straight, unoptimized calculation would be like:
        *    *dnoise_dx = -8.0f * t20 * t0 * x0 * graddotp2(gx0, gy0, x0, y0) + t40 * gx0;
        *    *dnoise_dy = -8.0f * t20 * t0 * y0 * graddotp2(gx0, gy0, x0, y0) + t40 * gy0;
        *    *dnoise_dx += -8.0f * t21 * t1 * x1 * graddotp2(gx1, gy1, x1, y1) + t41 * gx1;
        *    *dnoise_dy += -8.0f * t21 * t1 * y1 * graddotp2(gx1, gy1, x1, y1) + t41 * gy1;
        *    *dnoise_dx += -8.0f * t22 * t2 * x2 * graddotp2(gx2, gy2, x2, y2) + t42 * gx2;
        *    *dnoise_dy += -8.0f * t22 * t2 * y2 * graddotp2(gx2, gy2, x2, y2) + t42 * gy2;
        */
        T temp0 = t20 * t0 * graddotp2(gx0, gy0, x0, y0);
        T dx = temp0 * x0;
        T dy = temp0 * y0;

        T temp1 = t21 * t1 * graddotp2(gx1, gy1, x1, y1);
        dx += temp1 * x1;
        dy += temp1 * y1;

        T temp2 = t22 * t2 * graddotp2(gx2, gy2, x2, y2);
        dx += temp2 * x2;
        dy += temp2 * y2;

        dx *= T(-8);
        dy *= T(-8);

        /* This corrects a bug in the original implementation */
        dx += t40 * gx0 + t41 * gx1 + t42 * gx2;
        dy += t40 * gy0 + t41 * gy1 + t42 * gy2;

        dx *= T(40); /* Scale derivative to match the noise scaling */
        dy *= T(40);

        return detail::ValueDerivativePair<T, 2>(noise, Vec2<T>(dx, dy));
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    detail::ValueDerivativePair<T, 2> SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec2<T>& coords, const Vec2I& period, T angle, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        int px = period.x;
        int py = period.y;

        /* Sine and cosine for the gradient rotation angle */
        T sin_t = sin(angle);
        T cos_t = cos(angle);

        /* Skew the input space to determine which simplex cell we're in */
        T s = (x + y) * m_F2; /* Hairy factor for 2D */
        T xs = x + s;
        T ys = y + s;
        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);

        T t = T(i + j) * m_G2;
        T X0 = i - t; /* Unskew the cell origin back to (x,y) space */
        T Y0 = j - t;
        T x0 = x - X0; /* The x,y distances from the cell origin */
        T y0 = y - Y0;

        /* For the 2D case, the simplex shape is an equilateral triangle.
        * Determine which simplex we are in. */
        int i1, j1; /* Offsets for second (middle) corner of simplex in (i,j) coords */
        if(x0 > y0) { i1 = 1; j1 = 0; } /* lower triangle, XY order: (0,0)->(1,0)->(1,1) */
        else { i1 = 0; j1 = 1; }      /* upper triangle, YX order: (0,0)->(0,1)->(1,1) */

                                      /* A step of (1,0) in (i,j) means a step of (1-c,-c) in (x,y), and
                                      * a step of (0,1) in (i,j) means a step of (-c,1-c) in (x,y), where
                                      * c = (3-sqrt(3))/6   */
        T x1 = x0 - i1 + m_G2; /* Offsets for middle corner in (x,y) unskewed coords */
        T y1 = y0 - j1 + m_G2;
        T x2 = x0 - T(1) + T(2) * m_G2; /* Offsets for last corner in (x,y) unskewed coords */
        T y2 = y0 - T(1) + T(2) * m_G2;

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t iip2 = Util::applyPeriod(i + 1, px);
        uint32_t jjp2 = Util::applyPeriod(j + 1, py);

        /* Calculate the contribution from the three corners */
        T t0 = T(0.5) - x0 * x0 - y0 * y0;
        T t20, t40;
        T n0, n1, n2; /* Noise contributions from the three simplex corners */
        T gx0, gy0, gx1, gy1, gx2, gy2; /* Gradients at simplex corners */
        if(t0 < T(0)) t40 = t20 = t0 = n0 = gx0 = gy0 = T(0); /* No influence */
        else
        {
            gradrot2(hasher(iip0 + hasher(jjp0)), sin_t, cos_t, gx0, gy0);
            t20 = t0 * t0;
            t40 = t20 * t20;
            n0 = t40 * graddotp2(gx0, gy0, x0, y0);
        }

        T t1 = T(0.5) - x1 * x1 - y1 * y1;
        T t21, t41;
        if(t1 < T(0)) t21 = t41 = t1 = n1 = gx1 = gy1 = T(0); /* No influence */
        else
        {
            gradrot2(hasher(iip1 + hasher(jjp1)), sin_t, cos_t, gx1, gy1);
            t21 = t1 * t1;
            t41 = t21 * t21;
            n1 = t41 * graddotp2(gx1, gy1, x1, y1);
        }

        T t2 = T(0.5) - x2 * x2 - y2 * y2;
        T t22, t42;
        if(t2 < T(0)) t42 = t22 = t2 = n2 = gx2 = gy2 = T(0); /* No influence */
        else
        {
            gradrot2(hasher(iip2 + hasher(jjp2)), sin_t, cos_t, gx2, gy2);
            t22 = t2 * t2;
            t42 = t22 * t22;
            n2 = t42 * graddotp2(gx2, gy2, x2, y2);
        }

        /* Add contributions from each corner to get the final noise value.
        * The result is scaled to return values in the interval [-1,1]. */
        T noise = T(40) * (n0 + n1 + n2);

        /* Compute derivative */
        /*  A straight, unoptimised calculation would be like:
        *    *dnoise_dx = -8.0f * t20 * t0 * x0 * graddotp2(gx0, gy0, x0, y0) + t40 * gx0;
        *    *dnoise_dy = -8.0f * t20 * t0 * y0 * graddotp2(gx0, gy0, x0, y0) + t40 * gy0;
        *    *dnoise_dx += -8.0f * t21 * t1 * x1 * graddotp2(gx1, gy1, x1, y1) + t41 * gx1;
        *    *dnoise_dy += -8.0f * t21 * t1 * y1 * graddotp2(gx1, gy1, x1, y1) + t41 * gy1;
        *    *dnoise_dx += -8.0f * t22 * t2 * x2 * graddotp2(gx2, gy2, x2, y2) + t42 * gx2;
        *    *dnoise_dy += -8.0f * t22 * t2 * y2 * graddotp2(gx2, gy2, x2, y2) + t42 * gy2;
        */
        T temp0 = t20 * t0 * graddotp2(gx0, gy0, x0, y0);
        T dx = temp0 * x0;
        T dy = temp0 * y0;

        T temp1 = t21 * t1 * graddotp2(gx1, gy1, x1, y1);
        dx += temp1 * x1;
        dy += temp1 * y1;

        T temp2 = t22 * t2 * graddotp2(gx2, gy2, x2, y2);
        dx += temp2 * x2;
        dy += temp2 * y2;

        dx *= T(-8);
        dy *= T(-8);

        /* This corrects a bug in the original implementation */
        dx += t40 * gx0 + t41 * gx1 + t42 * gx2;
        dy += t40 * gy0 + t41 * gy1 + t42 * gy2;

        dx *= T(40); /* Scale derivative to match the noise scaling */
        dy *= T(40);

        return detail::ValueDerivativePair<T, 2>(noise, Vec2<T>(dx, dy));
    }

    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec3<T>& coords, const Vec3I& period, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        T z = coords.z;
        int px = period.x;
        int py = period.y;
        int pz = period.z;

        // Skew the input space to determine which simplex cell we're in
        T s = (x + y + z)*m_F3; // Very nice and simple skew factor for 3D
        T xs = x + s;
        T ys = y + s;
        T zs = z + s;
        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);
        int k = Util::fastFloor(zs);

        T t = T(i + j + k)*m_G3;
        T X0 = i - t; // Unskew the cell origin back to (x,y,z) space
        T Y0 = j - t;
        T Z0 = k - t;
        T x0 = x - X0; // The x,y,z distances from the cell origin
        T y0 = y - Y0;
        T z0 = z - Z0;

        // For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        // Determine which simplex we are in.
        int i1, j1, k1; // Offsets for second corner of simplex in (i,j,k) coords
        int i2, j2, k2; // Offsets for third corner of simplex in (i,j,k) coords

                        /* This code would benefit from a backport from the GLSL version! */
        if(x0 >= y0)
        {
            if(y0 >= z0)
            {
                i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
            } // X Y Z order
            else if(x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } // X Z Y order
            else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } // Z X Y order
        }
        else
        { // x0<y0
            if(y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } // Z Y X order
            else if(x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } // Y Z X order
            else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } // Y X Z order
        }

        // A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        // a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
        // a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
        // c = 1/6.

        T x1 = x0 - i1 + m_G3; // Offsets for second corner in (x,y,z) coords
        T y1 = y0 - j1 + m_G3;
        T z1 = z0 - k1 + m_G3;
        T x2 = x0 - i2 + T(2)*m_G3; // Offsets for third corner in (x,y,z) coords
        T y2 = y0 - j2 + T(2)*m_G3;
        T z2 = z0 - k2 + T(2)*m_G3;
        T x3 = x0 - T(1) + T(3)*m_G3; // Offsets for last corner in (x,y,z) coords
        T y3 = y0 - T(1) + T(3)*m_G3;
        T z3 = z0 - T(1) + T(3)*m_G3;

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t kkp0 = Util::applyPeriod(k, pz);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t kkp1 = Util::applyPeriod(k + k1, pz);
        uint32_t iip2 = Util::applyPeriod(i + i2, px);
        uint32_t jjp2 = Util::applyPeriod(j + j2, py);
        uint32_t kkp2 = Util::applyPeriod(k + k2, pz);
        uint32_t iip3 = Util::applyPeriod(i + 1, px);
        uint32_t jjp3 = Util::applyPeriod(j + 1, py);
        uint32_t kkp3 = Util::applyPeriod(k + 1, pz);

        T n0, n1, n2, n3; // Noise contributions from the four corners

                          // Calculate the contribution from the four corners
        T t0 = T(0.6) - x0*x0 - y0*y0 - z0*z0;
        if(t0 < T(0)) n0 = T(0);
        else
        {
            t0 *= t0;
            n0 = t0 * t0 * grad3(hasher(iip0 + hasher(jjp0 + hasher(kkp0))), x0, y0, z0);
        }

        T t1 = T(0.6) - x1*x1 - y1*y1 - z1*z1;
        if(t1 < T(0)) n1 = T(0);
        else
        {
            t1 *= t1;
            n1 = t1 * t1 * grad3(hasher(iip1 + hasher(jjp1 + hasher(kkp1))), x1, y1, z1);
        }

        T t2 = T(0.6) - x2*x2 - y2*y2 - z2*z2;
        if(t2 < T(0)) n2 = T(0);
        else
        {
            t2 *= t2;
            n2 = t2 * t2 * grad3(hasher(iip2 + hasher(jjp2 + hasher(kkp2))), x2, y2, z2);
        }

        T t3 = T(0.6) - x3*x3 - y3*y3 - z3*z3;
        if(t3 < T(0)) n3 = T(0);
        else
        {
            t3 *= t3;
            n3 = t3 * t3 * grad3(hasher(iip3 + hasher(jjp3 + hasher(kkp3))), x3, y3, z3);
        }

        // Add contributions from each corner to get the final noise value.
        // The result is scaled to stay just inside [-1,1]
        return T(32) * (n0 + n1 + n2 + n3); // TODO: The scale factor is preliminary! 
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    detail::ValueDerivativePair<T, 3> SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec3<T>& coords, const Vec3I& period, ComputeDerivativeTag, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        T z = coords.z;
        int px = period.x;
        int py = period.y;
        int pz = period.z;

        /* Skew the input space to determine which simplex cell we're in */
        T s = (x + y + z)*m_F3; /* Very nice and simple skew factor for 3D */
        T xs = x + s;
        T ys = y + s;
        T zs = z + s;
        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);
        int k = Util::fastFloor(zs);

        T t = T(i + j + k)*m_G3;
        T X0 = i - t; /* Unskew the cell origin back to (x,y,z) space */
        T Y0 = j - t;
        T Z0 = k - t;
        T x0 = x - X0; /* The x,y,z distances from the cell origin */
        T y0 = y - Y0;
        T z0 = z - Z0;

        /* For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        * Determine which simplex we are in. */
        int i1, j1, k1; /* Offsets for second corner of simplex in (i,j,k) coords */
        int i2, j2, k2; /* Offsets for third corner of simplex in (i,j,k) coords */

                        /* TODO: This code would benefit from a backport from the GLSL version! */
        if(x0 >= y0)
        {
            if(y0 >= z0)
            {
                i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
            } /* X Y Z order */
            else if(x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } /* X Z Y order */
            else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } /* Z X Y order */
        }
        else // x0<y0
        {
            if(y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } /* Z Y X order */
            else if(x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } /* Y Z X order */
            else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } /* Y X Z order */
        }

        /* A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        * a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
        * a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
        * c = 1/6.   */

        T x1 = x0 - i1 + m_G3; /* Offsets for second corner in (x,y,z) coords */
        T y1 = y0 - j1 + m_G3;
        T z1 = z0 - k1 + m_G3;
        T x2 = x0 - i2 + T(2) * m_G3; /* Offsets for third corner in (x,y,z) coords */
        T y2 = y0 - j2 + T(2) * m_G3;
        T z2 = z0 - k2 + T(2) * m_G3;
        T x3 = x0 - T(1) + T(3) * m_G3; /* Offsets for last corner in (x,y,z) coords */
        T y3 = y0 - T(1) + T(3) * m_G3;
        T z3 = z0 - T(1) + T(3) * m_G3;

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t kkp0 = Util::applyPeriod(k, pz);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t kkp1 = Util::applyPeriod(k + k1, pz);
        uint32_t iip2 = Util::applyPeriod(i + i2, px);
        uint32_t jjp2 = Util::applyPeriod(j + j2, py);
        uint32_t kkp2 = Util::applyPeriod(k + k2, pz);
        uint32_t iip3 = Util::applyPeriod(i + 1, px);
        uint32_t jjp3 = Util::applyPeriod(j + 1, py);
        uint32_t kkp3 = Util::applyPeriod(k + 1, pz);

        /* Calculate the contribution from the four corners */
        T t0 = T(0.6) - x0*x0 - y0*y0 - z0*z0;
        T t20, t40;
        T n0, n1, n2, n3; /* Noise contributions from the four simplex corners */
        T gx0, gy0, gz0, gx1, gy1, gz1; /* Gradients at simplex corners */
        T gx2, gy2, gz2, gx3, gy3, gz3;
        if(t0 < T(0)) n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = T(0);
        else
        {
            grad3d(hasher(iip0 + hasher(jjp0 + hasher(kkp0))), gx0, gy0, gz0);
            t20 = t0 * t0;
            t40 = t20 * t20;
            n0 = t40 * (gx0 * x0 + gy0 * y0 + gz0 * z0);
        }

        T t1 = T(0.6) - x1*x1 - y1*y1 - z1*z1;
        T t21, t41;
        if(t1 < T(0)) n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = T(0);
        else
        {
            grad3d(hasher(iip1 + hasher(jjp1 + hasher(kkp1))), gx1, gy1, gz1);
            t21 = t1 * t1;
            t41 = t21 * t21;
            n1 = t41 * (gx1 * x1 + gy1 * y1 + gz1 * z1);
        }

        T t2 = T(0.6) - x2*x2 - y2*y2 - z2*z2;
        T t22, t42;
        if(t2 < T(0)) n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = T(0);
        else
        {
            grad3d(hasher(iip2 + hasher(jjp2 + hasher(kkp2))), gx2, gy2, gz2);
            t22 = t2 * t2;
            t42 = t22 * t22;
            n2 = t42 * (gx2 * x2 + gy2 * y2 + gz2 * z2);
        }

        T t3 = T(0.6) - x3*x3 - y3*y3 - z3*z3;
        T t23, t43;
        if(t3 < T(0)) n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = T(0);
        else
        {
            grad3d(hasher(iip3 + hasher(jjp3 + hasher(kkp3))), gx3, gy3, gz3);
            t23 = t3 * t3;
            t43 = t23 * t23;
            n3 = t43 * (gx3 * x3 + gy3 * y3 + gz3 * z3);
        }

        /*  Add contributions from each corner to get the final noise value.
        * The result is scaled to return values in the range [-1,1] */
        T noise = T(28) * (n0 + n1 + n2 + n3);

        /* Compute derivative */
        /*  A straight, unoptimized calculation would be like:
        *    *dnoise_dx = -8.0f * t20 * t0 * x0 * graddotp3(gx0, gy0, gz0, x0, y0, z0) + t40 * gx0;
        *    *dnoise_dy = -8.0f * t20 * t0 * y0 * graddotp3(gx0, gy0, gz0, x0, y0, z0) + t40 * gy0;
        *    *dnoise_dz = -8.0f * t20 * t0 * z0 * graddotp3(gx0, gy0, gz0, x0, y0, z0) + t40 * gz0;
        *    *dnoise_dx += -8.0f * t21 * t1 * x1 * graddotp3(gx1, gy1, gz1, x1, y1, z1) + t41 * gx1;
        *    *dnoise_dy += -8.0f * t21 * t1 * y1 * graddotp3(gx1, gy1, gz1, x1, y1, z1) + t41 * gy1;
        *    *dnoise_dz += -8.0f * t21 * t1 * z1 * graddotp3(gx1, gy1, gz1, x1, y1, z1) + t41 * gz1;
        *    *dnoise_dx += -8.0f * t22 * t2 * x2 * graddotp3(gx2, gy2, gz2, x2, y2, z2) + t42 * gx2;
        *    *dnoise_dy += -8.0f * t22 * t2 * y2 * graddotp3(gx2, gy2, gz2, x2, y2, z2) + t42 * gy2;
        *    *dnoise_dz += -8.0f * t22 * t2 * z2 * graddotp3(gx2, gy2, gz2, x2, y2, z2) + t42 * gz2;
        *    *dnoise_dx += -8.0f * t23 * t3 * x3 * graddotp3(gx3, gy3, gz3, x3, y3, z3) + t43 * gx3;
        *    *dnoise_dy += -8.0f * t23 * t3 * y3 * graddotp3(gx3, gy3, gz3, x3, y3, z3) + t43 * gy3;
        *    *dnoise_dz += -8.0f * t23 * t3 * z3 * graddotp3(gx3, gy3, gz3, x3, y3, z3) + t43 * gz3;
        */

        T temp0 = t20 * t0 * graddotp3(gx0, gy0, gz0, x0, y0, z0);
        T dx = temp0 * x0;
        T dy = temp0 * y0;
        T dz = temp0 * z0;

        T temp1 = t21 * t1 * graddotp3(gx1, gy1, gz1, x1, y1, z1);
        dx += temp1 * x1;
        dy += temp1 * y1;
        dz += temp1 * z1;

        T temp2 = t22 * t2 * graddotp3(gx2, gy2, gz2, x2, y2, z2);
        dx += temp2 * x2;
        dy += temp2 * y2;
        dz += temp2 * z2;

        T temp3 = t23 * t3 * graddotp3(gx3, gy3, gz3, x3, y3, z3);
        dx += temp3 * x3;
        dy += temp3 * y3;
        dz += temp3 * z3;

        dx *= T(-8);
        dy *= T(-8);
        dz *= T(-8);

        /* This corrects a bug in the original implementation */
        dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3;
        dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3;
        dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3;

        dx *= T(28); /* Scale derivative to match the noise scaling */
        dy *= T(28);
        dz *= T(28);

        return detail::ValueDerivativePair<T, 3>(noise, Vec3<T>(dx, dy, dz));
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    detail::ValueDerivativePair<T, 3> SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec3<T>& coords, const Vec3I& period, T angle, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        T z = coords.z;
        int px = period.x;
        int py = period.y;
        int pz = period.z;

        /* Sine and cosine for the gradient rotation angle */
        T sin_t = sin(angle);
        T cos_t = cos(angle);

        /* Skew the input space to determine which simplex cell we're in */
        T s = (x + y + z)*m_F3; /* Very nice and simple skew factor for 3D */
        T xs = x + s;
        T ys = y + s;
        T zs = z + s;
        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);
        int k = Util::fastFloor(zs);

        T t = T(i + j + k)*m_G3;
        T X0 = i - t; /* Unskew the cell origin back to (x,y,z) space */
        T Y0 = j - t;
        T Z0 = k - t;
        T x0 = x - X0; /* The x,y,z distances from the cell origin */
        T y0 = y - Y0;
        T z0 = z - Z0;

        /* For the 3D case, the simplex shape is a slightly irregular tetrahedron.
        * Determine which simplex we are in. */
        int i1, j1, k1; /* Offsets for second corner of simplex in (i,j,k) coords */
        int i2, j2, k2; /* Offsets for third corner of simplex in (i,j,k) coords */

                        /* TODO: This code would benefit from a backport from the GLSL version! */
        if(x0 >= y0)
        {
            if(y0 >= z0)
            {
                i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 1; k2 = 0;
            } /* X Y Z order */
            else if(x0 >= z0) { i1 = 1; j1 = 0; k1 = 0; i2 = 1; j2 = 0; k2 = 1; } /* X Z Y order */
            else { i1 = 0; j1 = 0; k1 = 1; i2 = 1; j2 = 0; k2 = 1; } /* Z X Y order */
        }
        else
        { // x0<y0
            if(y0 < z0) { i1 = 0; j1 = 0; k1 = 1; i2 = 0; j2 = 1; k2 = 1; } /* Z Y X order */
            else if(x0 < z0) { i1 = 0; j1 = 1; k1 = 0; i2 = 0; j2 = 1; k2 = 1; } /* Y Z X order */
            else { i1 = 0; j1 = 1; k1 = 0; i2 = 1; j2 = 1; k2 = 0; } /* Y X Z order */
        }

        /* A step of (1,0,0) in (i,j,k) means a step of (1-c,-c,-c) in (x,y,z),
        * a step of (0,1,0) in (i,j,k) means a step of (-c,1-c,-c) in (x,y,z), and
        * a step of (0,0,1) in (i,j,k) means a step of (-c,-c,1-c) in (x,y,z), where
        * c = 1/6.   */

        T x1 = x0 - i1 + m_G3; /* Offsets for second corner in (x,y,z) coords */
        T y1 = y0 - j1 + m_G3;
        T z1 = z0 - k1 + m_G3;
        T x2 = x0 - i2 + 2.0f * m_G3; /* Offsets for third corner in (x,y,z) coords */
        T y2 = y0 - j2 + 2.0f * m_G3;
        T z2 = z0 - k2 + 2.0f * m_G3;
        T x3 = x0 - 1.0f + 3.0f * m_G3; /* Offsets for last corner in (x,y,z) coords */
        T y3 = y0 - 1.0f + 3.0f * m_G3;
        T z3 = z0 - 1.0f + 3.0f * m_G3;

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t kkp0 = Util::applyPeriod(k, pz);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t kkp1 = Util::applyPeriod(k + k1, pz);
        uint32_t iip2 = Util::applyPeriod(i + i2, px);
        uint32_t jjp2 = Util::applyPeriod(j + j2, py);
        uint32_t kkp2 = Util::applyPeriod(k + k2, pz);
        uint32_t iip3 = Util::applyPeriod(i + 1, px);
        uint32_t jjp3 = Util::applyPeriod(j + 1, py);
        uint32_t kkp3 = Util::applyPeriod(k + 1, pz);

        /* Calculate the contribution from the four corners */
        T t0 = T(0.6) - x0*x0 - y0*y0 - z0*z0;
        T t20, t40;
        T n0, n1, n2, n3; /* Noise contributions from the four simplex corners */
        T gx0, gy0, gz0, gx1, gy1, gz1; /* Gradients at simplex corners */
        T gx2, gy2, gz2, gx3, gy3, gz3;
        if(t0 < T(0)) n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = T(0);
        else
        {
            gradrot3(hasher(iip0 + hasher(jjp0 + hasher(kkp0))), sin_t, cos_t, gx0, gy0, gz0);
            t20 = t0 * t0;
            t40 = t20 * t20;
            n0 = t40 * graddotp3(gx0, gy0, gz0, x0, y0, z0);
        }

        T t1 = T(0.6) - x1*x1 - y1*y1 - z1*z1;
        T t21, t41;
        if(t1 < T(0)) n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = T(0);
        else
        {
            gradrot3(hasher(iip1 + hasher(jjp1 + hasher(kkp1))), sin_t, cos_t, gx1, gy1, gz1);
            t21 = t1 * t1;
            t41 = t21 * t21;
            n1 = t41 * graddotp3(gx1, gy1, gz1, x1, y1, z1);
        }

        T t2 = T(0.6) - x2*x2 - y2*y2 - z2*z2;
        T t22, t42;
        if(t2 < T(0)) n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = T(0);
        else
        {
            gradrot3(hasher(iip2 + hasher(jjp2 + hasher(kkp2))), sin_t, cos_t, gx2, gy2, gz2);
            t22 = t2 * t2;
            t42 = t22 * t22;
            n2 = t42 * graddotp3(gx2, gy2, gz2, x2, y2, z2);
        }

        T t3 = T(0.6) - x3*x3 - y3*y3 - z3*z3;
        T t23, t43;
        if(t3 < T(0)) n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = T(0);
        else
        {
            gradrot3(hasher(iip3 + hasher(jjp3 + hasher(kkp3))), sin_t, cos_t, gx3, gy3, gz3);
            t23 = t3 * t3;
            t43 = t23 * t23;
            n3 = t43 * graddotp3(gx3, gy3, gz3, x3, y3, z3);
        }

        /*  Add contributions from each corner to get the final noise value.
        * The result is scaled to return values in the range [-1,1] */
        T noise = T(28) * (n0 + n1 + n2 + n3);

        /* Compute derivative */
        /*  A straight, unoptimized calculation would be like:
        *    *dnoise_dx = -8.0f * t20 * t0 * x0 * graddotp3(gx0, gy0, gz0, x0, y0, z0) + t40 * gx0;
        *    *dnoise_dy = -8.0f * t20 * t0 * y0 * graddotp3(gx0, gy0, gz0, x0, y0, z0) + t40 * gy0;
        *    *dnoise_dz = -8.0f * t20 * t0 * z0 * graddotp3(gx0, gy0, gz0, x0, y0, z0) + t40 * gz0;
        *    *dnoise_dx += -8.0f * t21 * t1 * x1 * graddotp3(gx1, gy1, gz1, x1, y1, z1) + t41 * gx1;
        *    *dnoise_dy += -8.0f * t21 * t1 * y1 * graddotp3(gx1, gy1, gz1, x1, y1, z1) + t41 * gy1;
        *    *dnoise_dz += -8.0f * t21 * t1 * z1 * graddotp3(gx1, gy1, gz1, x1, y1, z1) + t41 * gz1;
        *    *dnoise_dx += -8.0f * t22 * t2 * x2 * graddotp3(gx2, gy2, gz2, x2, y2, z2) + t42 * gx2;
        *    *dnoise_dy += -8.0f * t22 * t2 * y2 * graddotp3(gx2, gy2, gz2, x2, y2, z2) + t42 * gy2;
        *    *dnoise_dz += -8.0f * t22 * t2 * z2 * graddotp3(gx2, gy2, gz2, x2, y2, z2) + t42 * gz2;
        *    *dnoise_dx += -8.0f * t23 * t3 * x3 * graddotp3(gx3, gy3, gz3, x3, y3, z3) + t43 * gx3;
        *    *dnoise_dy += -8.0f * t23 * t3 * y3 * graddotp3(gx3, gy3, gz3, x3, y3, z3) + t43 * gy3;
        *    *dnoise_dz += -8.0f * t23 * t3 * z3 * graddotp3(gx3, gy3, gz3, x3, y3, z3) + t43 * gz3;
        */

        T temp0 = t20 * t0 * graddotp3(gx0, gy0, gz0, x0, y0, z0);
        T dx = temp0 * x0;
        T dy = temp0 * y0;
        T dz = temp0 * z0;

        T temp1 = t21 * t1 * graddotp3(gx1, gy1, gz1, x1, y1, z1);
        dx += temp1 * x1;
        dy += temp1 * y1;
        dz += temp1 * z1;

        T temp2 = t22 * t2 * graddotp3(gx2, gy2, gz2, x2, y2, z2);
        dx += temp2 * x2;
        dy += temp2 * y2;
        dz += temp2 * z2;

        T temp3 = t23 * t3 * graddotp3(gx3, gy3, gz3, x3, y3, z3);
        dx += temp3 * x3;
        dy += temp3 * y3;
        dz += temp3 * z3;

        dx *= T(-8);
        dy *= T(-8);
        dz *= T(-8);

        /* This corrects a bug in the original implementation */
        dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3;
        dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3;
        dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3;

        dx *= T(28); /* Scale derivative to match the noise scaling */
        dy *= T(28);
        dz *= T(28);

        return detail::ValueDerivativePair<T, 3>(noise, Vec3<T>(dx, dy, dz));
    }

    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    T SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec4<T>& coords, const Vec4I& period, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        T z = coords.z;
        T w = coords.w;
        int px = period.x;
        int py = period.y;
        int pz = period.z;
        int pw = period.w;

        // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
        T s = (x + y + z + w) * m_F4; // Factor for 4D skewing
        T xs = x + s;
        T ys = y + s;
        T zs = z + s;
        T ws = w + s;

        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);
        int k = Util::fastFloor(zs);
        int l = Util::fastFloor(ws);

        T t = (i + j + k + l) * m_G4; // Factor for 4D unskewing
        T X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
        T Y0 = j - t;
        T Z0 = k - t;
        T W0 = l - t;

        T x0 = x - X0;  // The x,y,z,w distances from the cell origin
        T y0 = y - Y0;
        T z0 = z - Z0;
        T w0 = w - W0;

        // For the 4D case, the simplex is a 4D shape I won't even try to describe.
        // To find out which of the 24 possible simplices we're in, we need to
        // determine the magnitude ordering of x0, y0, z0 and w0.
        // The method below is a good way of finding the ordering of x,y,z,w and
        // then find the correct traversal order for the simplex we’re in.
        // First, six pair-wise comparisons are performed between each possible pair
        // of the four coordinates, and the results are used to add up binary bits
        // for an integer index.
        int c1 = (x0 > y0) ? 32 : 0;
        int c2 = (x0 > z0) ? 16 : 0;
        int c3 = (y0 > z0) ? 8 : 0;
        int c4 = (x0 > w0) ? 4 : 0;
        int c5 = (y0 > w0) ? 2 : 0;
        int c6 = (z0 > w0) ? 1 : 0;
        int c = c1 & c2 & c3 & c4 & c5 & c6;

        int i1, j1, k1, l1; // The integer offsets for the second simplex corner
        int i2, j2, k2, l2; // The integer offsets for the third simplex corner
        int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

                            // simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
                            // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
                            // impossible. Only the 24 indices which have non-zero entries make any sense.
                            // We use a thresholding to set the coordinates in turn from the largest magnitude.
                            // The number 3 in the "simplex" array is at the position of the largest coordinate.
        i1 = m_simplex[c][0] >= 3u ? 1 : 0;
        j1 = m_simplex[c][1] >= 3u ? 1 : 0;
        k1 = m_simplex[c][2] >= 3u ? 1 : 0;
        l1 = m_simplex[c][3] >= 3u ? 1 : 0;
        // The number 2 in the "simplex" array is at the second largest coordinate.
        i2 = m_simplex[c][0] >= 2u ? 1 : 0;
        j2 = m_simplex[c][1] >= 2u ? 1 : 0;
        k2 = m_simplex[c][2] >= 2u ? 1 : 0;
        l2 = m_simplex[c][3] >= 2u ? 1 : 0;
        // The number 1 in the "simplex" array is at the second smallest coordinate.
        i3 = m_simplex[c][0] >= 1u ? 1 : 0;
        j3 = m_simplex[c][1] >= 1u ? 1 : 0;
        k3 = m_simplex[c][2] >= 1u ? 1 : 0;
        l3 = m_simplex[c][3] >= 1u ? 1 : 0;
        // The fifth corner has all coordinate offsets = 1, so no need to look that up.

        T x1 = x0 - i1 + m_G4; // Offsets for second corner in (x,y,z,w) coords
        T y1 = y0 - j1 + m_G4;
        T z1 = z0 - k1 + m_G4;
        T w1 = w0 - l1 + m_G4;
        T x2 = x0 - i2 + T(2)*m_G4; // Offsets for third corner in (x,y,z,w) coords
        T y2 = y0 - j2 + T(2)*m_G4;
        T z2 = z0 - k2 + T(2)*m_G4;
        T w2 = w0 - l2 + T(2)*m_G4;
        T x3 = x0 - i3 + T(3)*m_G4; // Offsets for fourth corner in (x,y,z,w) coords
        T y3 = y0 - j3 + T(3)*m_G4;
        T z3 = z0 - k3 + T(3)*m_G4;
        T w3 = w0 - l3 + T(3)*m_G4;
        T x4 = x0 - T(1) + T(4)*m_G4; // Offsets for last corner in (x,y,z,w) coords
        T y4 = y0 - T(1) + T(4)*m_G4;
        T z4 = z0 - T(1) + T(4)*m_G4;
        T w4 = w0 - T(1) + T(4)*m_G4;

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t kkp0 = Util::applyPeriod(k, pz);
        uint32_t llp0 = Util::applyPeriod(l, pw);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t kkp1 = Util::applyPeriod(k + k1, pz);
        uint32_t llp1 = Util::applyPeriod(l + l1, pw);
        uint32_t iip2 = Util::applyPeriod(i + i2, px);
        uint32_t jjp2 = Util::applyPeriod(j + j2, py);
        uint32_t kkp2 = Util::applyPeriod(k + k2, pz);
        uint32_t llp2 = Util::applyPeriod(l + l2, pw);
        uint32_t iip3 = Util::applyPeriod(i + i3, px);
        uint32_t jjp3 = Util::applyPeriod(j + j3, py);
        uint32_t kkp3 = Util::applyPeriod(k + k3, pz);
        uint32_t llp3 = Util::applyPeriod(l + l3, pw);
        uint32_t iip4 = Util::applyPeriod(i + 1, px);
        uint32_t jjp4 = Util::applyPeriod(j + 1, py);
        uint32_t kkp4 = Util::applyPeriod(k + 1, pz);
        uint32_t llp4 = Util::applyPeriod(l + 1, pw);

        T  n0, n1, n2, n3, n4; // Noise contributions from the five corners

                               // Calculate the contribution from the five corners
        T  t0 = T(0.6) - x0*x0 - y0*y0 - z0*z0 - w0*w0;
        if(t0 < T(0)) n0 = T(0);
        else
        {
            t0 *= t0;
            n0 = t0 * t0 * grad4(hasher(iip0 + hasher(jjp0 + hasher(kkp0 + hasher(llp0)))), x0, y0, z0, w0);
        }

        T  t1 = T(0.6) - x1*x1 - y1*y1 - z1*z1 - w1*w1;
        if(t1 < T(0)) n1 = T(0);
        else
        {
            t1 *= t1;
            n1 = t1 * t1 * grad4(hasher(iip1 + hasher(jjp1 + hasher(kkp1 + hasher(llp1)))), x1, y1, z1, w1);
        }

        T  t2 = T(0.6) - x2*x2 - y2*y2 - z2*z2 - w2*w2;
        if(t2 < T(0)) n2 = T(0);
        else
        {
            t2 *= t2;
            n2 = t2 * t2 * grad4(hasher(iip2 + hasher(jjp2 + hasher(kkp2 + hasher(llp2)))), x2, y2, z2, w2);
        }

        T  t3 = T(0.6) - x3*x3 - y3*y3 - z3*z3 - w3*w3;
        if(t3 < T(0)) n3 = T(0);
        else
        {
            t3 *= t3;
            n3 = t3 * t3 * grad4(hasher(iip3 + hasher(jjp3 + hasher(kkp3 + hasher(llp3)))), x3, y3, z3, w3);
        }

        T  t4 = T(0.6) - x4*x4 - y4*y4 - z4*z4 - w4*w4;
        if(t4 < T(0)) n4 = T(0);
        else
        {
            t4 *= t4;
            n4 = t4 * t4 * grad4(hasher(iip4 + hasher(jjp4 + hasher(kkp4 + hasher(llp4)))), x4, y4, z4, w4);
        }

        // Sum up and scale the result to cover the range [-1,1]
        return T(27) * (n0 + n1 + n2 + n3 + n4); // TODO: The scale factor is preliminary! 
    }
    template <class T, int Dim, class Hasher>
    template <int D, class SFINAE>
    detail::ValueDerivativePair<T, 4> SimplexNoise<T, Dim, Hasher>::rawNoise(const Vec4<T>& coords, const Vec4I& period, ComputeDerivativeTag, const Hasher& hasher)
    {
        T x = coords.x;
        T y = coords.y;
        T z = coords.z;
        T w = coords.w;
        int px = period.x;
        int py = period.y;
        int pz = period.z;
        int pw = period.w;

        // Skew the (x,y,z,w) space to determine which cell of 24 simplices we're in
        T s = (x + y + z + w) * m_F4; // Factor for 4D skewing
        T xs = x + s;
        T ys = y + s;
        T zs = z + s;
        T ws = w + s;
        int i = Util::fastFloor(xs);
        int j = Util::fastFloor(ys);
        int k = Util::fastFloor(zs);
        int l = Util::fastFloor(ws);

        T t = (i + j + k + l) * m_G4; // Factor for 4D unskewing
        T X0 = i - t; // Unskew the cell origin back to (x,y,z,w) space
        T Y0 = j - t;
        T Z0 = k - t;
        T W0 = l - t;

        T x0 = x - X0;  // The x,y,z,w distances from the cell origin
        T y0 = y - Y0;
        T z0 = z - Z0;
        T w0 = w - W0;

        // For the 4D case, the simplex is a 4D shape I won't even try to describe.
        // To find out which of the 24 possible simplices we're in, we need to
        // determine the magnitude ordering of x0, y0, z0 and w0.
        // The method below is a reasonable way of finding the ordering of x,y,z,w
        // and then find the correct traversal order for the simplex we’re in.
        // First, six pair-wise comparisons are performed between each possible pair
        // of the four coordinates, and then the results are used to add up binary
        // bits for an integer index into a precomputed lookup table, simplex[].
        int c1 = (x0 > y0) ? 32 : 0;
        int c2 = (x0 > z0) ? 16 : 0;
        int c3 = (y0 > z0) ? 8 : 0;
        int c4 = (x0 > w0) ? 4 : 0;
        int c5 = (y0 > w0) ? 2 : 0;
        int c6 = (z0 > w0) ? 1 : 0;
        int c = c1 & c2 & c3 & c4 & c5 & c6; // '&' is mostly faster than '+'

        int i1, j1, k1, l1; // The integer offsets for the second simplex corner
        int i2, j2, k2, l2; // The integer offsets for the third simplex corner
        int i3, j3, k3, l3; // The integer offsets for the fourth simplex corner

                            // simplex[c] is a 4-vector with the numbers 0, 1, 2 and 3 in some order.
                            // Many values of c will never occur, since e.g. x>y>z>w makes x<z, y<w and x<w
                            // impossible. Only the 24 indices which have non-zero entries make any sense.
                            // We use a thresholding to set the coordinates in turn from the largest magnitude.
                            // The number 3 in the "simplex" array is at the position of the largest coordinate.
        i1 = m_simplex[c][0] >= 3u ? 1 : 0;
        j1 = m_simplex[c][1] >= 3u ? 1 : 0;
        k1 = m_simplex[c][2] >= 3u ? 1 : 0;
        l1 = m_simplex[c][3] >= 3u ? 1 : 0;
        // The number 2 in the "simplex" array is at the second largest coordinate.
        i2 = m_simplex[c][0] >= 2u ? 1 : 0;
        j2 = m_simplex[c][1] >= 2u ? 1 : 0;
        k2 = m_simplex[c][2] >= 2u ? 1 : 0;
        l2 = m_simplex[c][3] >= 2u ? 1 : 0;
        // The number 1 in the "simplex" array is at the second smallest coordinate.
        i3 = m_simplex[c][0] >= 1u ? 1 : 0;
        j3 = m_simplex[c][1] >= 1u ? 1 : 0;
        k3 = m_simplex[c][2] >= 1u ? 1 : 0;
        l3 = m_simplex[c][3] >= 1u ? 1 : 0;
        // The fifth corner has all coordinate offsets = 1, so no need to look that up.

        T x1 = x0 - i1 + m_G4; // Offsets for second corner in (x,y,z,w) coords
        T y1 = y0 - j1 + m_G4;
        T z1 = z0 - k1 + m_G4;
        T w1 = w0 - l1 + m_G4;
        T x2 = x0 - i2 + T(2) * m_G4; // Offsets for third corner in (x,y,z,w) coords
        T y2 = y0 - j2 + T(2) * m_G4;
        T z2 = z0 - k2 + T(2) * m_G4;
        T w2 = w0 - l2 + T(2) * m_G4;
        T x3 = x0 - i3 + T(3) * m_G4; // Offsets for fourth corner in (x,y,z,w) coords
        T y3 = y0 - j3 + T(3) * m_G4;
        T z3 = z0 - k3 + T(3) * m_G4;
        T w3 = w0 - l3 + T(3) * m_G4;
        T x4 = x0 - T(1) + T(4) * m_G4; // Offsets for last corner in (x,y,z,w) coords
        T y4 = y0 - T(1) + T(4) * m_G4;
        T z4 = z0 - T(1) + T(4) * m_G4;
        T w4 = w0 - T(1) + T(4) * m_G4;

        uint32_t iip0 = Util::applyPeriod(i, px);
        uint32_t jjp0 = Util::applyPeriod(j, py);
        uint32_t kkp0 = Util::applyPeriod(k, pz);
        uint32_t llp0 = Util::applyPeriod(l, pw);
        uint32_t iip1 = Util::applyPeriod(i + i1, px);
        uint32_t jjp1 = Util::applyPeriod(j + j1, py);
        uint32_t kkp1 = Util::applyPeriod(k + k1, pz);
        uint32_t llp1 = Util::applyPeriod(l + l1, pw);
        uint32_t iip2 = Util::applyPeriod(i + i2, px);
        uint32_t jjp2 = Util::applyPeriod(j + j2, py);
        uint32_t kkp2 = Util::applyPeriod(k + k2, pz);
        uint32_t llp2 = Util::applyPeriod(l + l2, pw);
        uint32_t iip3 = Util::applyPeriod(i + i3, px);
        uint32_t jjp3 = Util::applyPeriod(j + j3, py);
        uint32_t kkp3 = Util::applyPeriod(k + k3, pz);
        uint32_t llp3 = Util::applyPeriod(l + l3, pw);
        uint32_t iip4 = Util::applyPeriod(i + 1, px);
        uint32_t jjp4 = Util::applyPeriod(j + 1, py);
        uint32_t kkp4 = Util::applyPeriod(k + 1, pz);
        uint32_t llp4 = Util::applyPeriod(l + 1, pw);

        // Calculate the contribution from the five corners
        T t0 = T(0.6) - x0*x0 - y0*y0 - z0*z0 - w0*w0;
        T n0, n1, n2, n3, n4; // Noise contributions from the five corners
        T gx0, gy0, gz0, gw0, gx1, gy1, gz1, gw1; /* Gradients at simplex corners */
        T gx2, gy2, gz2, gw2, gx3, gy3, gz3, gw3, gx4, gy4, gz4, gw4;
        T t20, t21, t22, t23, t24;
        T t40, t41, t42, t43, t44;
        if(t0 < T(0)) n0 = t0 = t20 = t40 = gx0 = gy0 = gz0 = gw0 = T(0);
        else
        {
            t20 = t0 * t0;
            t40 = t20 * t20;
            grad4d(hasher(iip0 + hasher(jjp0 + hasher(kkp0 + hasher(llp0)))), gx0, gy0, gz0, gw0);
            n0 = t40 * (gx0 * x0 + gy0 * y0 + gz0 * z0 + gw0 * w0);
        }

        T t1 = T(0.6) - x1*x1 - y1*y1 - z1*z1 - w1*w1;
        if(t1 < T(0)) n1 = t1 = t21 = t41 = gx1 = gy1 = gz1 = gw1 = T(0);
        else
        {
            t21 = t1 * t1;
            t41 = t21 * t21;
            grad4d(hasher(iip1 + hasher(jjp1 + hasher(kkp1 + hasher(llp1)))), gx1, gy1, gz1, gw1);
            n1 = t41 * (gx1 * x1 + gy1 * y1 + gz1 * z1 + gw1 * w1);
        }

        T t2 = T(0.6) - x2*x2 - y2*y2 - z2*z2 - w2*w2;
        if(t2 < T(0)) n2 = t2 = t22 = t42 = gx2 = gy2 = gz2 = gw2 = T(0);
        else
        {
            t22 = t2 * t2;
            t42 = t22 * t22;
            grad4d(hasher(iip2 + hasher(jjp2 + hasher(kkp2 + hasher(llp2)))), gx2, gy2, gz2, gw2);
            n2 = t42 * (gx2 * x2 + gy2 * y2 + gz2 * z2 + gw2 * w2);
        }

        T t3 = T(0.6) - x3*x3 - y3*y3 - z3*z3 - w3*w3;
        if(t3 < T(0)) n3 = t3 = t23 = t43 = gx3 = gy3 = gz3 = gw3 = T(0);
        else
        {
            t23 = t3 * t3;
            t43 = t23 * t23;
            grad4d(hasher(iip3 + hasher(jjp3 + hasher(kkp3 + hasher(llp3)))), gx3, gy3, gz3, gw3);
            n3 = t43 * (gx3 * x3 + gy3 * y3 + gz3 * z3 + gw3 * w3);
        }

        T t4 = T(0.6) - x4*x4 - y4*y4 - z4*z4 - w4*w4;
        if(t4 < T(0)) n4 = t4 = t24 = t44 = gx4 = gy4 = gz4 = gw4 = T(0);
        else
        {
            t24 = t4 * t4;
            t44 = t24 * t24;
            grad4d(hasher(iip4 + hasher(jjp4 + hasher(kkp4 + hasher(llp4)))), gx4, gy4, gz4, gw4);
            n4 = t44 * (gx4 * x4 + gy4 * y4 + gz4 * z4 + gw4 * w4);
        }

        // Sum up and scale the result to cover the range [-1,1]
        T noise = T(27) * (n0 + n1 + n2 + n3 + n4); // TODO: The scale factor is preliminary!

                                                    /* Compute derivative, if requested by supplying non-null pointers
                                                    * for the last four arguments */
                                                    /*  A straight, unoptimised calculation would be like:
                                                    *    T dx = -8.0f * t20 * t0 * x0 * dot(gx0, gy0, gz0, gw0, x0, y0, z0, w0) + t40 * gx0;
                                                    *    T dy = -8.0f * t20 * t0 * y0 * dot(gx0, gy0, gz0, gw0, x0, y0, z0, w0) + t40 * gy0;
                                                    *    T dz = -8.0f * t20 * t0 * z0 * dot(gx0, gy0, gz0, gw0, x0, y0, z0, w0) + t40 * gz0;
                                                    *    T dw = -8.0f * t20 * t0 * w0 * dot(gx0, gy0, gz0, gw0, x0, y0, z0, w0) + t40 * gw0;

                                                    *    dx += -8.0f * t21 * t1 * x1 * dot(gx1, gy1, gz1, gw1, x1, y1, z1, w1) + t41 * gx1;
                                                    *    dy += -8.0f * t21 * t1 * y1 * dot(gx1, gy1, gz1, gw1, x1, y1, z1, w1) + t41 * gy1;
                                                    *    dz += -8.0f * t21 * t1 * z1 * dot(gx1, gy1, gz1, gw1, x1, y1, z1, w1) + t41 * gz1;
                                                    *    dw += -8.0f * t21 * t1 * w1 * dot(gx1, gy1, gz1, gw1, x1, y1, z1, w1) + t41 * gw1;

                                                    *    dx += -8.0f * t22 * t2 * x2 * dot(gx2, gy2, gz2, gw2, x2, y2, z2, w2) + t42 * gx2;
                                                    *    dy += -8.0f * t22 * t2 * y2 * dot(gx2, gy2, gz2, gw2, x2, y2, z2, w2) + t42 * gy2;
                                                    *    dz += -8.0f * t22 * t2 * z2 * dot(gx2, gy2, gz2, gw2, x2, y2, z2, w2) + t42 * gz2;
                                                    *    dw += -8.0f * t22 * t2 * w2 * dot(gx2, gy2, gz2, gw2, x2, y2, z2, w2) + t42 * gw2;

                                                    *    dx += -8.0f * t23 * t3 * x3 * dot(gx3, gy3, gz3, gw3, x3, y3, z3, w3) + t43 * gx3;
                                                    *    dy += -8.0f * t23 * t3 * y3 * dot(gx3, gy3, gz3, gw3, x3, y3, z3, w3) + t43 * gy3;
                                                    *    dz += -8.0f * t23 * t3 * z3 * dot(gx3, gy3, gz3, gw3, x3, y3, z3, w3) + t43 * gz3;
                                                    *    dw += -8.0f * t23 * t3 * w3 * dot(gx3, gy3, gz3, gw3, x3, y3, z3, w3) + t43 * gw3;

                                                    *    dx += -8.0f * t24 * t4 * x4 * dot(gx4, gy4, gz4, gw4, x4, y4, z4, w4) + t44 * gx4;
                                                    *    dy += -8.0f * t24 * t4 * y4 * dot(gx4, gy4, gz4, gw4, x4, y4, z4, w4) + t44 * gy4;
                                                    *    dz += -8.0f * t24 * t4 * z4 * dot(gx4, gy4, gz4, gw4, x4, y4, z4, w4) + t44 * gz4;
                                                    *    dw += -8.0f * t24 * t4 * w4 * dot(gx4, gy4, gz4, gw4, x4, y4, z4, w4) + t44 * gw4;
                                                    */
        T temp0 = t20 * t0 * graddotp4(gx0, gy0, gz0, gw0, x0, y0, z0, w0);
        T dx = temp0 * x0;
        T dy = temp0 * y0;
        T dz = temp0 * z0;
        T dw = temp0 * w0;

        T temp1 = t21 * t1 * graddotp4(gx1, gy1, gz1, gw1, x1, y1, z1, w1);
        dx += temp1 * x1;
        dy += temp1 * y1;
        dz += temp1 * z1;
        dw += temp1 * w1;

        T temp2 = t22 * t2 * graddotp4(gx2, gy2, gz2, gw2, x2, y2, z2, w2);
        dx += temp2 * x2;
        dy += temp2 * y2;
        dz += temp2 * z2;
        dw += temp2 * w2;

        T temp3 = t23 * t3 * graddotp4(gx3, gy3, gz3, gw3, x3, y3, z3, w3);
        dx += temp3 * x3;
        dy += temp3 * y3;
        dz += temp3 * z3;
        dw += temp3 * w3;

        T temp4 = t24 * t4 * graddotp4(gx4, gy4, gz4, gw4, x4, y4, z4, w4);
        dx += temp4 * x4;
        dy += temp4 * y4;
        dz += temp4 * z4;
        dw += temp4 * w4;

        dx *= T(-8);
        dy *= T(-8);
        dz *= T(-8);
        dw *= T(-8);

        dx += t40 * gx0 + t41 * gx1 + t42 * gx2 + t43 * gx3 + t44 * gx4;
        dy += t40 * gy0 + t41 * gy1 + t42 * gy2 + t43 * gy3 + t44 * gy4;
        dz += t40 * gz0 + t41 * gz1 + t42 * gz2 + t43 * gz3 + t44 * gz4;
        dw += t40 * gw0 + t41 * gw1 + t42 * gw2 + t43 * gw3 + t44 * gw4;

        dx *= T(27); /* Scale derivative to match the noise scaling */
        dy *= T(27);
        dz *= T(27);
        dw *= T(27);

        return detail::ValueDerivativePair<T, 4>(noise, Vec4<T>(dx, dy, dz, dw));
    }

    template <class T, int Dim, class Hasher>
    template <class Ret, class... Args>
    Ret SimplexNoise<T, Dim, Hasher>::fractal(const VectorType& coords, Args... args) const
    {
        Ret total = T(0);
        T amplitude = T(1);
        VectorType frequency = Super::m_scale;
        VectorTypeI period = Super::m_period;

        // We have to keep track of the sum of the amplitudes,
        // because each octave adds more, and we need a value in [-1, 1].
        T amplitudeSum = T(0);

        for(int i = 0; i < Super::m_octaves; ++i)
        {
            Ret current = rawNoise(coords * frequency, period, args..., m_hasher);
            total += current * amplitude;

            frequency *= 2;
            Super::doublePeriod(period);
            amplitudeSum += amplitude;
            amplitude *= Super::m_persistence;
        }

        total /= amplitudeSum;

        return total;
    }
}
