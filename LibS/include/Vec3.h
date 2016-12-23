#pragma once

#include "..\Fwd.h"

#include "Shape3.h"

#include <initializer_list>

namespace ls
{
    template <class T>
    class Vec3 : public Shape3<T>
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;

        T x, y, z;

        constexpr Vec3() = default;
        constexpr explicit Vec3(const T& _xyz) noexcept(std::is_nothrow_copy_constructible<T>::value);
        constexpr Vec3(const T& _x, const T& _y, const T& _z) noexcept(std::is_nothrow_copy_constructible<T>::value);

        constexpr static Vec3<T> zero() noexcept(std::is_nothrow_constructible<Vec3<T>, const T&>::value);

        constexpr Vec3(const Vec3<T>& other) = default;
        constexpr Vec3(Vec3<T>&& other) = default;

        Vec3<T>& operator=(const Vec3<T>& other) & = default;
        Vec3<T>& operator=(Vec3<T> && other) & = default;

        Vec3<T>& operator+=(const Vec3<T>& rhs) &;
        Vec3<T>& operator-=(const Vec3<T>& rhs) &;
        Vec3<T>& operator*=(const T& rhs) &;
        Vec3<T>& operator*=(const Vec3<T>& rhs) &;
        Vec3<T>& operator/=(const T& rhs) &;

        const T& operator[](int i) const;
        T& operator[](int i);

        template <class T2>
        explicit operator Vec3<T2>() const;

        constexpr T magnitude() const;
        constexpr T magnitudeSquared() const;
        constexpr T distance(const Vec3<T>& other) const;
        void normalize() &;
        Vec3<T> normalized() const;
        T dot(const Vec3<T>& rhs) const;
        Vec3<T> cross(const Vec3<T>& rhs) const;

        static constexpr Vec3<T> unitX() noexcept;
        static constexpr Vec3<T> unitY() noexcept;
        static constexpr Vec3<T> unitZ() noexcept;
    };

    template <class T>
    constexpr bool operator==(const Vec3<T>& lhs, const Vec3<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Vec3<T>& lhs, const Vec3<T>& rhs);
    template <class T>
    constexpr bool operator<(const Vec3<T>& lhs, const Vec3<T>& rhs);
    template <class T>
    constexpr bool operator>(const Vec3<T>& lhs, const Vec3<T>& rhs);
    template <class T>
    constexpr bool operator<=(const Vec3<T>& lhs, const Vec3<T>& rhs);
    template <class T>
    constexpr bool operator>=(const Vec3<T>& lhs, const Vec3<T>& rhs);

    template <class T>
    constexpr Vec3<T> operator-(const Vec3<T>& vector);

    template <class T1, class T2, class R = decltype(std::declval<T1>() + std::declval<T2>())>
    constexpr Vec3<R> operator+(const Vec3<T1>& lhs, const Vec3<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() - std::declval<T2>())>
    constexpr Vec3<R> operator-(const Vec3<T1>& lhs, const Vec3<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const T2& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const T2& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec3<R> operator*(const T1& lhs, const Vec3<T2>& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec3<R> operator/(const T1& lhs, const Vec3<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec3<R> operator*(const Vec3<T1>& lhs, const Vec3<T2>& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec3<R> operator/(const Vec3<T1>& lhs, const Vec3<T2>& rhs);

    using Vec3D = Vec3<double>;
    using Vec3F = Vec3<float>;
    using Vec3I = Vec3<int>;

    extern template class Vec3<double>;
    extern template class Vec3<float>;
    extern template class Vec3<int>;
}
#include "../src/Vec3.cpp"
