#include "..\include\Vec2.h"

namespace ls
{
    template <class T1, class T2, class R>
    constexpr Vec2<R> operator+(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x + rhs.x,
            lhs.y + rhs.y
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec2<R> operator-(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x - rhs.x,
            lhs.y - rhs.y
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec2<R> operator*(const Vec2<T1>& lhs, const T2& rhs)
    {
        return Vec2<R>(
            lhs.x * rhs,
            lhs.y * rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec2<R> operator/(const Vec2<T1>& lhs, const T2& rhs)
    {
        return Vec2<R>(
            lhs.x / rhs,
            lhs.y / rhs
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec2<R> operator*(const T1& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs * rhs.x,
            lhs * rhs.y
            );
    }
    template <class T1, class T2, class R>
    constexpr Vec2<R> operator/(const T1& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs / rhs.x,
            lhs / rhs.y
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec2<R> operator*(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x * rhs.x,
            lhs.y * rhs.y
            );
    }

    template <class T1, class T2, class R>
    constexpr Vec2<R> operator/(const Vec2<T1>& lhs, const Vec2<T2>& rhs)
    {
        return Vec2<R>(
            lhs.x / rhs.x,
            lhs.y / rhs.y
            );
    }

    template <class T>
    constexpr T Vec2<T>::magnitude() const
    {
        return static_cast<T>(std::sqrt(magnitudeSquared()));
    }
    template <class T>
    constexpr T Vec2<T>::magnitudeSquared() const
    {
        return x * x + y * y;
    }
    template <class T>
    void Vec2<T>::normalize() &
    {
        const T invertedMagnitude = T(1) / magnitude();
        x *= invertedMagnitude;
        y *= invertedMagnitude;
    }
    template <class T>
    Vec2<T> Vec2<T>::normalized() const
    {
        Vec2<T> result(*this);
        result.normalize();
        return result;
    }
    template <class T>
    constexpr T Vec2<T>::dot(const Vec2<T>& rhs) const
    {
        return x * rhs.x + y * rhs.y;
    }
    template <class T>
    constexpr T Vec2<T>::cross(const Vec2<T>& rhs) const
    {
        return x * rhs.y - y * rhs.x;
    }
    template <class T>
    Vec2<T> Vec2<T>::projected(const Vec2<T>& axis) const
    {
        const Vec2<T> n = axis.normalized();
        return n*dot(n);
    }
    template <class T>
    void Vec2<T>::mirror(const Vec2<T>& mirror)
    {
        *this = -*this + T(2)*mirror*(dot(mirror));
    }
    template <class T>
    Vec2<T> Vec2<T>::mirrored(const Vec2<T>& mirror) const
    {
        Vec2<T> result(*this);
        result.mirror(mirror);
        return result;
    }
    template <class T>
    void Vec2<T>::rotate(const Angle2<T>& angle)
    {
        rotate(direction(angle));
    }
    template <class T>
    Vec2<T> Vec2<T>::rotated(const Angle2<T>& angle) const
    {
        return rotated(direction(angle));
    }
    template <class T>
    void Vec2<T>::rotate(const Vec2<T>& vector)
    {
        const T xx = x;
        const T yy = y;

        x = xx*vector.x - yy*vector.y;
        y = xx*vector.y + yy*vector.x;
    }
    template <class T>
    Vec2<T> Vec2<T>::rotated(const Vec2<T>& vector) const
    {
        Vec2<T> result(*this);
        result.rotate(vector);
        return result;
    }
    template <class T>
    Vec2<T> Vec2<T>::normalLeft() const
    {
        return Vec2<T>(-y, x).normalized();
    }
    template <class T>
    Vec2<T> Vec2<T>::normalRight() const
    {
        return Vec2<T>(y, -x).normalized();
    }
    template <class T>
    Vec2<T> Vec2<T>::normal() const //left one
    {
        return normalLeft();
    }
    template <class T>
    constexpr Angle2<T> Vec2<T>::angle() const
    {
        return Angle2<T>::atan2(y, x);
    }
    template <class T>
    constexpr Angle2<T> Vec2<T>::angle(const Vec2<T>& other) const
    {
        return Angle2<T>::acos(dot(other) / (other.magnitude() * magnitude()));
    }


    template <class T>
    constexpr Vec2<T>::Vec2(const T& _xy) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_xy),
        y(_xy)
    {
    }
    template <class T>
    constexpr Vec2<T>::Vec2(const T& _x, const T& _y) noexcept(std::is_nothrow_copy_constructible<T>::value) :
        x(_x),
        y(_y)
    {
    }

    template <class T>
    Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& rhs) &
    {
        x += rhs.x;
        y += rhs.y;
        return *this;
    }
    template <class T>
    Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& rhs) &
    {
        x -= rhs.x;
        y -= rhs.y;
        return *this;
    }
    template <class T>
    Vec2<T>& Vec2<T>::operator*=(const T& rhs) &
    {
        x *= rhs;
        y *= rhs;
        return *this;
    }
    template <class T>
    Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& rhs) &
    {
        x *= rhs.x;
        y *= rhs.y;
        return *this;
    }
    template <class T>
    Vec2<T>& Vec2<T>::operator/=(const T& rhs) &
    {
        x /= rhs;
        y /= rhs;
        return *this;
    }
    template <class T>
    Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& rhs) &
    {
        x /= rhs.x;
        y /= rhs.y;
        return *this;
    }

    template <class T>
    const T& Vec2<T>::operator[](int i) const
    {
        //static constexpr T Vec2<T>::* m_members[2] = {&Vec2<T>::x, &Vec2<T>::y};
        //return this->*(m_members[i]);
        switch(i)
        {
        case 0: return x;
        case 1: return y;
        }
        return y;
    }
    template <class T>
    T& Vec2<T>::operator[](int i)
    {
        switch(i)
        {
        case 0: return x;
        case 1: return y;
        }
        return y;
    }

    template <class T>
    template <class T2>
    Vec2<T>::operator Vec2<T2>() const
    {
        return Vec2<T2>(static_cast<T2>(x), static_cast<T2>(y));
    }

    template <class T>
    constexpr T Vec2<T>::distance(const Vec2<T>& other) const
    {
        return (other - *this).magnitude();
    }

    template <class T>
    constexpr Vec2<T> Vec2<T>::direction(const Angle2<T>& angle) noexcept(std::is_nothrow_constructible<Vec2<T>, const T&, const T&>::value)
    {
        return Vec2<T>(angle.cos(), angle.sin());
    }
    template <class T>
    constexpr Vec2<T> Vec2<T>::zero() noexcept(std::is_nothrow_constructible<Vec2<T>, const T&>::value)
    {
        return Vec2<T>(T(0));
    }

    template <class T>
    constexpr Vec2<T> Vec2<T>::unitX() noexcept
    {
        return{1, 0};
    }
    template <class T>
    constexpr Vec2<T> Vec2<T>::unitY() noexcept
    {
        return{0, 1};
    }

    template <class T>
    constexpr bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return std::tie(lhs.x, lhs.y) == std::tie(rhs.x, rhs.y);
    }
    template <class T>
    constexpr bool operator!=(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return !(lhs == rhs);
    }
    template <class T>
    constexpr bool operator<(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return std::tie(x, y) < std::tie(rhs.x, rhs.y);
    }
    template <class T>
    constexpr bool operator>(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return rhs < lhs;
    }
    template <class T>
    constexpr bool operator<=(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    constexpr bool operator>=(const Vec2<T>& lhs, const Vec2<T>& rhs)
    {
        return !(lhs < rhs);
    }
    template <class T>
    constexpr Vec2<T> operator-(const Vec2<T>& vector)
    {
        return Vec2<T>(-vector.x, -vector.y);
    }
}