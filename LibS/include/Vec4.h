#pragma once

#include "..\Fwd.h"

#include "Shape4.h"

#include <initializer_list>

namespace ls
{
    template <class T>
    class Vec4 : public Shape4<T>
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;

        T x, y, z, w;

        constexpr Vec4() = default;
        constexpr explicit Vec4(const T& _xyzw) noexcept(std::is_nothrow_copy_constructible<T>::value);
        constexpr Vec4(const T& _x, const T& _y, const T& _z, const T& _w) noexcept(std::is_nothrow_copy_constructible<T>::value);

        constexpr static Vec4<T> zero() noexcept(std::is_nothrow_constructible<Vec4<T>, const T&>::value);

        constexpr Vec4(const Vec4<T>& other) = default;
        constexpr Vec4(Vec4<T>&& other) = default;

        Vec4<T>& operator=(const Vec4<T>& other) & = default;
        Vec4<T>& operator=(Vec4<T> && other) & = default;

        Vec4<T>& operator+=(const Vec4<T>& rhs) &;
        Vec4<T>& operator-=(const Vec4<T>& rhs) &;
        Vec4<T>& operator*=(const T& rhs) &;
        Vec4<T>& operator*=(const Vec4<T>& rhs) &;
        Vec4<T>& operator/=(const T& rhs) &;

        const T& operator[](int i) const;
        T& operator[](int i);

        template <class T2>
        explicit operator Vec4<T2>() const;

        constexpr T magnitude() const;
        constexpr T magnitudeSquared() const;
        constexpr T distance(const Vec4<T>& other) const;
        void normalize() &;
        Vec4<T> normalized() const;

        static constexpr Vec4<T> unitX() noexcept;
        static constexpr Vec4<T> unitY() noexcept;
        static constexpr Vec4<T> unitZ() noexcept;
        static constexpr Vec4<T> unitW() noexcept;
    };

    template <class T>
    constexpr bool operator==(const Vec4<T>& lhs, const Vec4<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Vec4<T>& lhs, const Vec4<T>& rhs);
    template <class T>
    constexpr bool operator<(const Vec4<T>& lhs, const Vec4<T>& rhs);
    template <class T>
    constexpr bool operator>(const Vec4<T>& lhs, const Vec4<T>& rhs);
    template <class T>
    constexpr bool operator<=(const Vec4<T>& lhs, const Vec4<T>& rhs);
    template <class T>
    constexpr bool operator>=(const Vec4<T>& lhs, const Vec4<T>& rhs);

    template <class T>
    constexpr Vec4<T> operator-(const Vec4<T>& vector);

    template <class T1, class T2, class R = decltype(std::declval<T1>() + std::declval<T2>())>
    constexpr Vec4<R> operator+(const Vec4<T1>& lhs, const Vec4<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() - std::declval<T2>())>
    constexpr Vec4<R> operator-(const Vec4<T1>& lhs, const Vec4<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const T2& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const T2& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec4<R> operator*(const T1& lhs, const Vec4<T2>& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec4<R> operator/(const T1& lhs, const Vec4<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec4<R> operator*(const Vec4<T1>& lhs, const Vec4<T2>& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec4<R> operator/(const Vec4<T1>& lhs, const Vec4<T2>& rhs);

    using Vec4D = Vec4<double>;
    using Vec4F = Vec4<float>;
    using Vec4I = Vec4<int>;

    extern template class Vec4<double>;
    extern template class Vec4<float>;
    extern template class Vec4<int>;
}
#include "../src/Vec4.cpp"
