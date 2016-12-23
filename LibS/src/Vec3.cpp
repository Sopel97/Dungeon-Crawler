#include "..\include\Vec3.h"

namespace ls
{
    template <class T>
    constexpr Vec3<T>::Vec3(const T& _xyz) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_xyz),
        y(_xyz),
        z(_xyz)
    {
    }
    template <class T>
    constexpr Vec3<T>::Vec3(const T& _x, const T& _y, const T& _z) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_x), 
        y(_y), 
        z(_z)
    {
    }
    template <class T>
    constexpr Vec3<T> Vec3<T>::zero() noexcept(std::is_nothrow_constructible<Vec3<T>, const T&>::value)
    {
        return Vec3<T>(T(0));
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator+(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator-(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const T2& rhs)
    {
        return Vec3<R>(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const T2& rhs)
    {
        return Vec3<R>(
            lhs.x / rhs,
            lhs.y / rhs,
            lhs.z / rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator*(const T1& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec3<R> operator/(const T1& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const Vec3<T2>& rhs)
    {
        return Vec3<R>(
            lhs.x / rhs.x,
            lhs.y / rhs.y,
            lhs.z / rhs.z
            );
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& rhs) &
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& rhs) &
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator*=(const T& rhs) &
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& rhs) &
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        return *this;
    }
    template <class T>
    Vec3<T>& Vec3<T>::operator/=(const T& rhs) &
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        return *this;
    }

    template <class T>
    const T& Vec3<T>::operator[](int i) const
    {
        switch(i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        return z;
    }
    template <class T>
    T& Vec3<T>::operator[](int i)
    {
        switch(i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        }
        return z;
    }

    template <class T>
    template <class T2>
    Vec3<T>::operator Vec3<T2>() const
    {
        return Vec3<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z));
    }

    template <class T>
    constexpr T Vec3<T>::magnitude() const
    {
        return static_cast<T>(std::sqrt(magnitudeSquared()));
    }
    template <class T>
    constexpr T Vec3<T>::magnitudeSquared() const
    {
        return static_cast<T>((x * x) + (y * y) + (z * z));
    }
    template <class T>
    constexpr T Vec3<T>::distance(const Vec3<T>& other) const
    {
        return (other-*this).magnitude();
    }
    template <class T>
    void Vec3<T>::normalize() &
    {
        const T invMag = T(1) / magnitude();
        x *= invMag;
        y *= invMag;
        z *= invMag;
    }
    template <class T>
    Vec3<T> Vec3<T>::normalized() const
    {
        Vec3<T> result(*this);
        result.normalize();
        return result;
    }
    template <class T>
    T Vec3<T>::dot(const Vec3<T>& rhs) const
    {
        return x*rhs.x + y*rhs.y + z*rhs.z;
    }
    template <class T>
    Vec3<T> Vec3<T>::cross(const Vec3<T>& rhs) const
    {
        return Vec3<T>(y*rhs.z - z*rhs.y, z*rhs.x - x*rhs.z, x*rhs.y - y*rhs.x);
    }

    template <class T>
    constexpr Vec3<T> Vec3<T>::unitX() noexcept
    {
        return{1, 0, 0};
    }
    template <class T>
    constexpr Vec3<T> Vec3<T>::unitY() noexcept
    {
        return{0, 1, 0};
    }
    template <class T>
    constexpr Vec3<T> Vec3<T>::unitZ() noexcept
    {
        return{0, 0, 1};
    }

    template <class T>
    constexpr bool operator==(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z) == std::tie(rhs.x, rhs.y, rhs.z);
    }
    template <class T>
    constexpr bool operator!=(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    constexpr bool operator<(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z) < std::tie(rhs.x, rhs.y, rhs.z);
    }
    template <class T>
    constexpr bool operator>(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    constexpr bool operator<=(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    constexpr bool operator>=(const Vec3<T>& lhs, const Vec3<T>& rhs)
    {
        return !(lhs < rhs);
    }
    template <class T>
    constexpr Vec3<T> operator-(const Vec3<T>& vector)
    {
        return Vec3<T>(-vector.x, -vector.y, -vector.z);
    }
}