#include "..\include\Vec4.h"

namespace ls
{
    template <class T>
    constexpr Vec4<T>::Vec4(const T& _xyzw) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_xyzw),
        y(_xyzw),
        z(_xyzw),
        w(_xyzw)
    {
    }
    template <class T>
    constexpr Vec4<T>::Vec4(const T& _x, const T& _y, const T& _z, const T& _w) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_x),
        y(_y),
        z(_z),
        w(_w)
    {
    }
    template <class T>
    constexpr Vec4<T> Vec4<T>::zero() noexcept(std::is_nothrow_constructible<Vec4<T>, const T&>::value)
    {
        return Vec4<T>(T(0));
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator+(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x + rhs.x,
            lhs.y + rhs.y,
            lhs.z + rhs.z,
            lhs.w + rhs.w
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator-(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x - rhs.x,
            lhs.y - rhs.y,
            lhs.z - rhs.z,
            lhs.w - rhs.w
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const T2& rhs)
    {
        return Vec4<R>(
            lhs.x * rhs,
            lhs.y * rhs,
            lhs.z * rhs,
            lhs.w * rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const T2& rhs)
    {
        return Vec4<R>(
            lhs.x / rhs,
            lhs.y / rhs,
            lhs.z / rhs,
            lhs.w / rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator*(const T1& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs * rhs.x,
            lhs * rhs.y,
            lhs * rhs.z,
            lhs * rhs.w
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec4<R> operator/(const T1& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs / rhs.x,
            lhs / rhs.y,
            lhs / rhs.z,
            lhs / rhs.w
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x * rhs.x,
            lhs.y * rhs.y,
            lhs.z * rhs.z,
            lhs.w * rhs.w
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const Vec4<T2>& rhs)
    {
        return Vec4<R>(
            lhs.x / rhs.x,
            lhs.y / rhs.y,
            lhs.z / rhs.z,
            lhs.w / rhs.w
            );
    }

    template <class T>
    Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& rhs) &
    {
        x += rhs.x;
        y += rhs.y;
        z += rhs.z;
        w += rhs.w;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& rhs) &
    {
        x -= rhs.x;
        y -= rhs.y;
        z -= rhs.z;
        w -= rhs.w;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator*=(const T& rhs) &
    {
        x *= rhs;
        y *= rhs;
        z *= rhs;
        w *= rhs;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator*=(const Vec4<T>& rhs) &
    {
        x *= rhs.x;
        y *= rhs.y;
        z *= rhs.z;
        w *= rhs.w;
        return *this;
    }
    template <class T>
    Vec4<T>& Vec4<T>::operator/=(const T& rhs) &
    {
        x /= rhs;
        y /= rhs;
        z /= rhs;
        w /= rhs;
        return *this;
    }

    template <class T>
    const T& Vec4<T>::operator[](int i) const
    {
        switch(i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
        return w;
    }
    template <class T>
    T& Vec4<T>::operator[](int i)
    {
        switch(i)
        {
        case 0: return x;
        case 1: return y;
        case 2: return z;
        case 3: return w;
        }
        return w;
    }
    
    template <class T>
    template <class T2>
    Vec4<T>::operator Vec4<T2>() const
    {
        return Vec4<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z), static_cast<T2>(w));
    }

    template <class T>
    constexpr T Vec4<T>::magnitude() const
    {
        return static_cast<T>(std::sqrt(magnitudeSquared()));
    }
    template <class T>
    constexpr T Vec4<T>::magnitudeSquared() const
    {
        return static_cast<T>((x*x) + (y*y) + (z*z) + (w*w));
    }
    template <class T>
    constexpr T Vec4<T>::distance(const Vec4<T>& other) const
    {
        return (other - *this).magnitude();
    }
    template <class T>
    void Vec4<T>::normalize() &
    {
        const T invMag = T(1) / magnitude();
        x *= invMag;
        y *= invMag;
        z *= invMag;
        w *= invMag;
    }
    template <class T>
    Vec4<T> Vec4<T>::normalized() const
    {
        Vec4<T> result(*this);
        result.normalize();
        return result;
    }

    template <class T>
    constexpr Vec4<T> Vec4<T>::unitX() noexcept
    {
        return{1, 0, 0, 0};
    }
    template <class T>
    constexpr Vec4<T> Vec4<T>::unitY() noexcept
    {
        return{0, 1, 0, 0};
    }
    template <class T>
    constexpr Vec4<T> Vec4<T>::unitZ() noexcept
    {
        return{0, 0, 1, 0};
    }
    template <class T>
    constexpr Vec4<T> Vec4<T>::unitW() noexcept
    {
        return{0, 0, 0, 1};
    }

    template <class T>
    constexpr bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) == std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
    }
    template <class T>
    constexpr bool operator!=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    constexpr bool operator<(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y, lhs.z, lhs.w) < std::tie(rhs.x, rhs.y, rhs.z, rhs.w);
    }
    template <class T>
    constexpr bool operator>(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    constexpr bool operator<=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    constexpr bool operator>=(const Vec4<T>& lhs, const Vec4<T>& rhs)
    {
        return !(lhs < rhs);
    }
    template <class T>
    constexpr Vec4<T> operator-(const Vec4<T>& vector)
    {
        return Vec4<T>(-vector.x, -vector.y, -vector.z, -vector.w);
    }
}