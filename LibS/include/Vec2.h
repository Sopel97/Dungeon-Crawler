#pragma once

#include "..\Fwd.h"

#include "Shape2.h"

#include <initializer_list>
#include <type_traits>

namespace ls
{
    template <class T>
    class Vec2 : public Shape2<T>
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;

        T x, y;

        constexpr Vec2() = default;
        constexpr explicit Vec2(const T& _xy) noexcept(std::is_nothrow_copy_constructible<T>::value);
        constexpr Vec2(const T& _x, const T& _y) noexcept(std::is_nothrow_copy_constructible<T>::value);
        
        constexpr static Vec2<T> direction(const Angle2<T>& angle) noexcept(std::is_nothrow_constructible<Vec2<T>, const T&, const T&>::value);
        constexpr static Vec2<T> zero() noexcept(std::is_nothrow_constructible<Vec2<T>, const T&>::value);

        constexpr Vec2(const Vec2<T>& other) = default;
        constexpr Vec2(Vec2<T>&& other) = default;

        Vec2<T>& operator=(const Vec2<T>& other) & = default;
        Vec2<T>& operator=(Vec2<T> && other) & = default;

        Vec2<T>& operator+=(const Vec2<T>& rhs) &;
        Vec2<T>& operator-=(const Vec2<T>& rhs) &;
        Vec2<T>& operator*=(const T& rhs) &;
        Vec2<T>& operator*=(const Vec2<T>& rhs) &;
        Vec2<T>& operator/=(const T& rhs) &;
        Vec2<T>& operator/=(const Vec2<T>& rhs) &;

        const T& operator[](int i) const;
        T& operator[](int i);

        template <class T2>
        explicit operator Vec2<T2>() const;

        constexpr T magnitude() const;
        constexpr T magnitudeSquared() const;
        void normalize() &;
        Vec2<T> normalized() const;
        Vec2<T> normalLeft() const;
        Vec2<T> normalRight() const;
        Vec2<T> normal() const; //one of above two. Should be used only when it makes no difference which one is used.
        constexpr T dot(const Vec2<T>& rhs) const;
        constexpr T cross(const Vec2<T>& rhs) const;
        Vec2<T> projected(const Vec2<T>& axis) const;
        void mirror(const Vec2<T>& mirror);
        Vec2<T> mirrored(const Vec2<T>& mirror) const;
        void rotate(const Angle2<T>& angle);
        Vec2<T> rotated(const Angle2<T>& angle) const;
        void rotate(const Vec2<T>& vector);
        Vec2<T> rotated(const Vec2<T>& vector) const;
        constexpr Angle2<T> angle() const;
        constexpr Angle2<T> angle(const Vec2<T>& other) const;

        constexpr T distance(const Vec2<T>& other) const;

        static constexpr Vec2<T> unitX() noexcept;
        static constexpr Vec2<T> unitY() noexcept;

    };

    template <class T>
    constexpr bool operator==(const Vec2<T>& lhs, const Vec2<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Vec2<T>& lhs, const Vec2<T>& rhs);
    template <class T>
    constexpr bool operator<(const Vec2<T>& lhs, const Vec2<T>& rhs);
    template <class T>
    constexpr bool operator>(const Vec2<T>& lhs, const Vec2<T>& rhs);
    template <class T>
    constexpr bool operator<=(const Vec2<T>& lhs, const Vec2<T>& rhs);
    template <class T>
    constexpr bool operator>=(const Vec2<T>& lhs, const Vec2<T>& rhs);

    template <class T>
    constexpr Vec2<T> operator-(const Vec2<T>& vector);
    

    template <class T1, class T2, class R = decltype(std::declval<T1>() + std::declval<T2>())>
    constexpr Vec2<R> operator+(const Vec2<T1>& lhs, const Vec2<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() - std::declval<T2>())>
    constexpr Vec2<R> operator-(const Vec2<T1>& lhs, const Vec2<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec2<R> operator*(const Vec2<T1>& lhs, const T2& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec2<R> operator/(const Vec2<T1>& lhs, const T2& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec2<R> operator*(const T1& lhs, const Vec2<T2>& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec2<R> operator/(const T1& lhs, const Vec2<T2>& rhs);

    template <class T1, class T2, class R = decltype(std::declval<T1>() * std::declval<T2>())>
    constexpr Vec2<R> operator*(const Vec2<T1>& lhs, const Vec2<T2>& rhs);
    template <class T1, class T2, class R = decltype(std::declval<T1>() / std::declval<T2>())>
    constexpr Vec2<R> operator/(const Vec2<T1>& lhs, const Vec2<T2>& rhs);

    using Vec2D = Vec2<double>;
    using Vec2F = Vec2<float>;
    using Vec2I = Vec2<int>;

    extern template class Vec2<double>;
    extern template class Vec2<float>;
    extern template class Vec2<int>;
}

#include "../src/Vec2.cpp"
