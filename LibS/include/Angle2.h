#pragma once

#include "..\Fwd.h"

namespace ls
{
    template <class T>
    class Angle2 //directed angle 
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;

        constexpr Angle2() = default;

        constexpr Angle2(const Angle2<T>& other) = default;
        constexpr Angle2(Angle2<T>&& other) = default;

        constexpr static Angle2<T> radians(const T& rad) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value);
        constexpr static Angle2<T> degrees(const T& deg) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value);
        constexpr static Angle2<T> fullTurn() noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value);

        Angle2<T>& operator=(const Angle2<T>& other) & = default;
        Angle2<T>& operator=(Angle2<T> && other) & = default;

        Angle2<T>& operator+=(const Angle2<T>& rhs) &;
        Angle2<T>& operator-=(const Angle2<T>& rhs) &;
        Angle2<T>& operator*=(const T& rhs) &;
        Angle2<T>& operator/=(const T& rhs) &;

        void normalize() &;
        Angle2<T> normalized() const; // [0, 2pi)

        template <class T2>
        explicit operator Angle2<T2>() const;

        constexpr T sin() const;
        constexpr T cos() const;
        constexpr T tan() const;
        constexpr T cot() const;

        constexpr static Angle2<T> asin(const T& val);
        constexpr static Angle2<T> acos(const T& val);
        constexpr static Angle2<T> atan(const T& val);
        constexpr static Angle2<T> acot(const T& val);

        constexpr T sinh() const;
        constexpr T cosh() const;
        constexpr T tanh() const;
        constexpr T coth() const;

        constexpr static Angle2<T> asinh(const T& val);
        constexpr static Angle2<T> acosh(const T& val);
        constexpr static Angle2<T> atanh(const T& val);
        constexpr static Angle2<T> acoth(const T& val);

        constexpr static Angle2<T> atan2(const T& y, const T& x);

        constexpr T radians() const;
        constexpr T degrees() const;
    protected:
    private:
        T m_radians;

        constexpr Angle2(const T& rad) noexcept(std::is_nothrow_constructible<T, T>::value);
    };

    template <class T>
    constexpr bool operator==(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Angle2<T>& lhs, const Angle2<T>& rhs);

    template <class T>
    constexpr bool operator<(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator<=(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator>(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr bool operator>=(const Angle2<T>& lhs, const Angle2<T>& rhs);

    template <class T>
    constexpr Angle2<T> operator+(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& lhs, const Angle2<T>& rhs);
    template <class T>
    constexpr Angle2<T> operator*(const Angle2<T>& lhs, const T& rhs);
    template <class T>
    constexpr Angle2<T> operator/(const Angle2<T>& lhs, const T& rhs);

    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& angle);

    using Angle2D = Angle2<double>;
    using Angle2F = Angle2<float>;
    using Angle2I = Angle2<int>;

    extern template class Angle2<double>;
    extern template class Angle2<float>;
}

#include "../src/Angle2.cpp"
