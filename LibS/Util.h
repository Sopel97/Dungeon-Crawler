#pragma once

#include "Fwd.h"
#include "Constants.h"

#include "include\Vec2.h"

#include <vector>
#include <algorithm>
#include <cmath>
#include <functional>

namespace ls
{
    class OnScopeExit
    {
    public:
        template <class TFwd>
        OnScopeExit(TFwd&& f) : m_f(std::forward<TFwd>(f)) {}
        OnScopeExit(const OnScopeExit& rhs) = delete;
        OnScopeExit(OnScopeExit&& rhs) = delete;
        ~OnScopeExit() { m_f(); }
    private:
        std::function<void(void)> m_f;
    };

    class Util
    {
    public:

        template <class T, int Dim>
        struct VectorType
        {
        };

        template <class T>
        struct VectorType<T, 1>
        {
            using type = T;
        };
        template <class T>
        struct VectorType<T, 2>
        {
            using type = Vec2<T>;
        };
        template <class T>
        struct VectorType<T, 3>
        {
            using type = Vec3<T>;
        };
        template <class T>
        struct VectorType<T, 4>
        {
            using type = Vec4<T>;
        };


        //32-bit fnv-a
        static uint32_t hash(uint32_t val)
        {
            static constexpr uint32_t fnvPrime = 16777619;
            static constexpr uint32_t offsetBasis = 2166136261u;

            uint32_t hash = offsetBasis;

            hash ^= val & 0b1111u;
            hash *= fnvPrime;
            val >>= 4u;

            hash ^= val & 0b1111u;
            hash *= fnvPrime;
            val >>= 4u;

            hash ^= val & 0b1111u;
            hash *= fnvPrime;
            val >>= 4u;

            hash ^= val;
            hash *= fnvPrime;

            return hash;
        }

        // evaluates v%p but converts negative results so that they are not symetric to positive ones
        static uint32_t applyPeriod(int v, int p)
        {
            v %= p;
            uint32_t vp = (v >= 0) ? v : v + p;
            return vp;
        }

        template <class T>
        static inline int fastFloor(const T& value)
        {
            int i = (int)value; /* truncate */
            return i - (value < 0.0);   /* convert trunc to floor */
        }
        template <class T>
        static inline Vec2I fastFloor(const Vec2<T>& value)
        {
            return Vec2I(fastFloor(value.x), fastFloor(value.y));
        }
        template <class T>
        static inline typename std::enable_if<std::is_integral<T>::value, T>::type mod(const T& value, const T& div)
        {
            T r = value % div;
            if (r < T(0)) r += div;
            return r;
        }
        template <class T>
        static inline typename std::enable_if<std::is_floating_point<T>::value, T>::type mod(const T& value, const T& div)
        {
            using std::abs;
            using std::floor;

            T i = abs(floor(value / div));
            T r = value - i * div;
            if (r < 0) r += div;
            return r;
        }
        template <class T>
        constexpr static inline T radiansToDegrees(const T radians)
        {
            return radians * (T(180.0) / static_cast<T>(PI));
        }
        template <class T>
        constexpr static inline T degreesToRadians(const T degrees)
        {
            return degrees * (static_cast<T>(PI) / T(180.0));
        }
        template <class T>
        constexpr static inline T clamp(T a, T mi, T ma)
        {
            using std::max;
            using std::min;

            return max(min(a, ma), mi);
        }

    protected:
    private:
        Util() {}
        virtual ~Util() {}

    };
}
