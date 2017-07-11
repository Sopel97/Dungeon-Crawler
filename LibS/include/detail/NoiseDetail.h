#pragma once

#include <memory>

#include "../../Util.h"
#include "../Vec2.h"
#include "../Vec3.h"
#include "../Vec4.h"

namespace ls
{
    namespace
    {
        class OriginalPerlinPermTable
        {
        private:
            static constexpr unsigned char m_defaultPerm[256] = {
                151,160,137,91,90,15,
                131,13,201,95,96,53,194,233,7,225,140,36,103,30,69,142,8,99,37,240,21,10,23,
                190, 6,148,247,120,234,75,0,26,197,62,94,252,219,203,117,35,11,32,57,177,33,
                88,237,149,56,87,174,20,125,136,171,168, 68,175,74,165,71,134,139,48,27,166,
                77,146,158,231,83,111,229,122,60,211,133,230,220,105,92,41,55,46,245,40,244,
                102,143,54, 65,25,63,161, 1,216,80,73,209,76,132,187,208, 89,18,169,200,196,
                135,130,116,188,159,86,164,100,109,198,173,186, 3,64,52,217,226,250,124,123,
                5,202,38,147,118,126,255,82,85,212,207,206,59,227,47,16,58,17,182,189,28,42,
                223,183,170,213,119,248,152, 2,44,154,163, 70,221,153,101,155,167, 43,172,9,
                129,22,39,253, 19,98,108,110,79,113,224,232,178,185, 112,104,218,246,97,228,
                251,34,242,193,238,210,144,12,191,179,162,241, 81,51,145,235,249,14,239,107,
                49,192,214, 31,181,199,106,157,184, 84,204,176,115,121,50,45,127, 4,150,254,
                138,236,205,93,222,114,67,29,24,72,243,141,128,195,78,66,215,61,156,180
            };

        public:
            OriginalPerlinPermTable() = default;

            uint32_t operator()(uint32_t x) const
            {
                return m_defaultPerm[x & 0xFF];
            }
        };
    }

    template <int N, class IntType>
    class PermTable
    {
        static_assert(std::is_unsigned<IntType>::value, "Must be an unsigned type");
    private:
        std::unique_ptr<IntType[]> m_perm;

        PermTable(std::unique_ptr<IntType[]>&& perm) :
            m_perm(std::move(perm))
        {

        }

    public:
        template <class RNG>
        static PermTable<N, IntType> shuffled(RNG&& rng)
        {
            std::unique_ptr<IntType[]> perm = std::make_unique<IntType[]>(N);
            std::iota(perm.get(), perm.get() + N, 0);
            std::shuffle(perm.get(), perm.get() + N, std::forward<RNG>(rng));
            return PermTable<N, IntType>(std::move(perm));
        }

        IntType operator()(IntType x) const
        {
            return m_perm[x % N];
        }
    };

    namespace detail
    {
        template <class T, class U>
        inline T scaleResult(const T& val, const U& lowerBound, const U& upperBound)
        {
            return (val + T(1)) * U(0.5) * (upperBound - lowerBound) + T(lowerBound);
        }

        template <class T>
        inline T lerp(const T& a, const T& b, const T& t)
        {
            return a+t*(b-a);
        }

        template <class T>
        inline T fade(const T& t)
        {
            return (t * t * t * (t * (t * T(6) - T(15)) + T(10)));
        }

        template <class T, int Dim>
        class PerlinSimplexInterface
        {
        public:
            using ValueType = T;
            using VectorType = typename Util::VectorType<T, Dim>::type;
            using VectorTypeI = typename Util::VectorType<int, Dim>::type;

        protected:
            VectorType m_scale;
            VectorTypeI m_period;

            T m_persistence;
            T m_lowerBound;
            T m_upperBound;

            int m_octaves;

        public:
            PerlinSimplexInterface() :
                m_scale(1),
                m_persistence(1),
                m_lowerBound(-1),
                m_upperBound(1),
                m_octaves(1)
            {

            }

            void setScale(const VectorType& newScale)
            {
                m_scale = newScale;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, void>::type>
            void setScaleX(T newScaleX)
            {
                m_scale.x = newScaleX;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, void>::type>
            void setScaleY(T newScaleY)
            {
                m_scale.y = newScaleY;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 3, void>::type>
            void setScaleZ(T newScaleZ)
            {
                m_scale.z = newScaleZ;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 4, void>::type>
            void setScaleW(T newScaleZ)
            {
                m_scale.w = newScaleW;
            }

            void setPersistence(T newPersistence)
            {
                m_persistence = newPersistence;
            }
            void setLowerBound(T newLowerBound)
            {
                m_lowerBound = newLowerBound;
            }
            void setUpperBound(T newUpperBound)
            {
                m_upperBound = newUpperBound;
            }

            void setPeriod(const VectorTypeI& newPeriod)
            {
                m_period = newPeriod;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, void>::type>
            void setPeriodX(int newPeriodX)
            {
                m_period.x = newPeriodX;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, void>::type>
            void setPeriodY(int newPeriodY)
            {
                m_period.y = newPeriodY;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 3, void>::type>
            void setPeriodZ(int newPeriodZ)
            {
                m_period.z = newPeriodZ;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 4, void>::type>
            void setPeriodW(int newPeriodW)
            {
                m_period.w = newPeriodW;
            }

            void setOctaves(int newOctaves)
            {
                m_octaves = newOctaves;
            }

            void removePeriod()
            {
                m_period = VectorTypeI(std::numeric_limits<int>::max());
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, void>::type>
            void removePeriodX()
            {
                m_period.x = std::numeric_limits<int>::max();
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, void>::type>
            void removePeriodY()
            {
                m_period.y = std::numeric_limits<int>::max();
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 3, void>::type>
            void removePeriodZ()
            {
                m_period.z = std::numeric_limits<int>::max();
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 4, void>::type>
            void removePeriodW()
            {
                m_period.w = std::numeric_limits<int>::max();
            }

            const VectorType& scale() const
            {
                return m_scale;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, T>::type>
            T scaleX() const
            {
                return m_scale.x;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, T>::type>
            T scaleY() const
            {
                return m_scale.y;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 3, T>::type>
            T scaleZ() const
            {
                return m_scale.z;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 4, T>::type>
            T scaleW() const
            {
                return m_scale.w;
            }


            T persistence() const
            {
                return m_persistence;
            }
            T lowerBound() const
            {
                return m_lowerBound;
            }
            T upperBound() const
            {
                return m_upperBound;
            }

            const VectorTypeI& period() const
            {
                return m_period;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, int>::type>
            int periodX() const
            {
                return m_period.x;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 2, int>::type>
            int periodY() const
            {
                return m_period.y;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 3, int>::type>
            int periodZ() const
            {
                return m_period.z;
            }
            template <int D = Dim, class SFINAE = typename std::enable_if<D >= 4, int>::type>
            int periodW() const
            {
                return m_period.w;
            }

            int octaves() const
            {
                return m_octaves;
            }

            void doublePeriod(int& period) const
            {
                if(period < std::numeric_limits<int>::max() / 2) period *= 2;
            }
            void doublePeriod(Vec2<int>& period) const
            {
                if(period.x < std::numeric_limits<int>::max() / 2) period.x *= 2;
                if(period.y < std::numeric_limits<int>::max() / 2) period.y *= 2;
            }
            void doublePeriod(Vec3<int>& period) const
            {
                if(period.x < std::numeric_limits<int>::max() / 2) period.x *= 2;
                if(period.y < std::numeric_limits<int>::max() / 2) period.y *= 2;
                if(period.z < std::numeric_limits<int>::max() / 2) period.z *= 2;
            }
            void doublePeriod(Vec4<int>& period) const
            {
                if(period.x < std::numeric_limits<int>::max() / 2) period.x *= 2;
                if(period.y < std::numeric_limits<int>::max() / 2) period.y *= 2;
                if(period.z < std::numeric_limits<int>::max() / 2) period.z *= 2;
                if(period.w < std::numeric_limits<int>::max() / 2) period.w *= 2;
            }
        };

        template <class T, int N>
        struct ValueDerivativePair
        {
            T value;
            typename Util::VectorType<T, N>::type derivative;
            
            ValueDerivativePair() = default;
            ValueDerivativePair(const T& v) noexcept(std::is_nothrow_copy_constructible<T>::value) :
                value(v),
                derivative(v)
            {

            }
            ValueDerivativePair(const T& v, typename Util::VectorType<T, N>::type d) noexcept(std::is_nothrow_copy_constructible<typename Util::VectorType<T, N>::type>::value) :
                value(v),
                derivative(d)
            {

            }

            ValueDerivativePair(const ValueDerivativePair<T, N>& other) = default;
            ValueDerivativePair(ValueDerivativePair<T, N>&& other) = default;

            ValueDerivativePair<T, N>& operator=(const ValueDerivativePair<T, N>& other) & = default;
            ValueDerivativePair<T, N>& operator=(ValueDerivativePair<T, N>&& other) & = default;

            ValueDerivativePair<T, N>& operator+=(const ValueDerivativePair<T, N>& rhs)
            {
                value += rhs.value;
                derivative += rhs.derivative;
                return *this;
            }
            ValueDerivativePair<T, N>& operator-=(const ValueDerivativePair<T, N>& rhs)
            {
                value -= rhs.value;
                derivative -= rhs.derivative;
                return *this;
            }
            ValueDerivativePair<T, N>& operator*=(const T& rhs)
            {
                value *= rhs;
                derivative *= rhs;
                return *this;
            }
            ValueDerivativePair<T, N>& operator/=(const T& rhs)
            {
                value /= rhs;
                derivative /= rhs;
                return *this;
            }
        };

        template <class T, int N>
        ValueDerivativePair<T, N> operator+(const ValueDerivativePair<T, N>& lhs, const ValueDerivativePair<T, N>& rhs)
        {
            ValueDerivativePair<T, N> result(lhs);
            result += rhs;
            return result;
        }
        template <class T, int N>
        ValueDerivativePair<T, N> operator-(const ValueDerivativePair<T, N>& lhs, const ValueDerivativePair<T, N>& rhs)
        {
            ValueDerivativePair<T, N> result(lhs);
            result -= rhs;
            return result;
        }
        template <class T, int N>
        ValueDerivativePair<T, N> operator*(const ValueDerivativePair<T, N>& lhs, const T& rhs)
        {
            ValueDerivativePair<T, N> result(lhs);
            result *= rhs;
            return result;
        }
        template <class T, int N>
        ValueDerivativePair<T, N> operator/(const ValueDerivativePair<T, N>& lhs, const T& rhs)
        {
            ValueDerivativePair<T, N> result(lhs);
            result /= rhs;
            return result;
        }
        template <class T, int N>
        ValueDerivativePair<T, N> operator-(const ValueDerivativePair<T, N>& arg)
        {
            ValueDerivativePair<T, N> result(arg);
            result *= T(-1);
            return result;
        }
    }
}