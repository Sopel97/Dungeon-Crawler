#include "..\include\Angle2.h"

#include "..\Constants.h"

#include "..\Util.h"

namespace ls
{
    template <class T>
    constexpr Angle2<T>::Angle2(const T& rad) noexcept(std::is_nothrow_constructible<T, T>::value) :
        m_radians(rad)
    {
    }

    template <class T>
    Angle2<T>& Angle2<T>::operator+=(const Angle2<T>& rhs) &
    {
        m_radians += rhs.m_radians;
        return *this;
    }
    template <class T>
    Angle2<T>& Angle2<T>::operator-=(const Angle2<T>& rhs) &
    {
        m_radians -= rhs.m_radians;
        return *this;
    }
    template <class T>
    Angle2<T>& Angle2<T>::operator*=(const T& rhs) &
    {
        m_radians *= rhs;
        return *this;
    }
    template <class T>
    Angle2<T>& Angle2<T>::operator/=(const T& rhs) &
    {
        m_radians /= rhs;
        return *this;
    }

    template <class T>
    void Angle2<T>::normalize() &
    {
        int fullTurns = Util::fastFloor(m_radians / fullTurn().m_radians);
        m_radians -= fullTurns * fullTurn().m_radians;
    }
    template <class T>
    Angle2<T> Angle2<T>::normalized() const
    {
        Angle2<T> result(*this);

        result.normalize();

        return result;
    }

    template <class T>
    template <class T2>
    Angle2<T>::operator Angle2<T2>() const
    {
        return Angle2<T2>::radians(static_cast<T2>(m_radians));
    }

    template <class T>
    constexpr T Angle2<T>::sin() const
    {
        using std::sin;

        return static_cast<T>(sin(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::cos() const
    {
        using std::cos;

        return static_cast<T>(cos(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::tan() const
    {
        using std::tan;

        return static_cast<T>(tan(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::cot() const
    {
        using std::tan;

        return T(1) / static_cast<T>(tan(m_radians));
    }

    template <class T>
    constexpr Angle2<T> Angle2<T>::asin(const T& val)
    {
        using std::asin;

        return Angle2<T>(static_cast<T>(asin(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acos(const T& val)
    {
        using std::acos;

        return Angle2<T>(static_cast<T>(acos(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::atan(const T& val)
    {
        using std::atan;

        return Angle2<T>(static_cast<T>(atan(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acot(const T& val)
    {
        using std::atan;

        return Angle2<T>(static_cast<T>(atan(T(1))) / val);
    }

    template <class T>
    constexpr T Angle2<T>::sinh() const
    {
        using std::sinh;

        return static_cast<T>(sinh(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::cosh() const
    {
        using std::cosh;

        return static_cast<T>(cosh(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::tanh() const
    {
        using std::tanh;

        return static_cast<T>(tanh(m_radians));
    }
    template <class T>
    constexpr T Angle2<T>::coth() const
    {
        using std::tanh;

        return T(1) / static_cast<T>(tanh(m_radians));
    }

    template <class T>
    constexpr Angle2<T> Angle2<T>::asinh(const T& val)
    {
        using std::atanh;

        return Angle2<T>(static_cast<T>(atanh(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acosh(const T& val)
    {
        using std::acosh;

        return Angle2<T>(static_cast<T>(acosh(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::atanh(const T& val)
    {
        using std::atanh;

        return Angle2<T>(static_cast<T>(atanh(val)));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::acoth(const T& val)
    {
        using std::atanh;

        return Angle2<T>(static_cast<T>(atanh(T(1))) / val);
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::atan2(const T& y, const T& x)
    {
        using std::atan2;

        return Angle2<T>(static_cast<T>(std::atan2(y, x)));
    }

    template <class T>
    constexpr T Angle2<T>::radians() const
    {
        return m_radians;
    }
    template <class T>
    constexpr T Angle2<T>::degrees() const
    {
        return Util::radiansToDegrees(m_radians);
    }

    template <class T>
    constexpr Angle2<T> Angle2<T>::radians(const T& rad) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value)
    {
        return Angle2<T>(rad);
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::degrees(const T& deg) noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value)
    {
        return Angle2<T>(Util::degreesToRadians(deg));
    }
    template <class T>
    constexpr Angle2<T> Angle2<T>::fullTurn() noexcept(std::is_nothrow_constructible<Angle2<T>, T>::value)
    {
        return Angle2<T>(Util::degreesToRadians(T(360)));
    }



    template <class T>
    constexpr bool operator==(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return lhs.radians() == rhs.radians();
    }
    template <class T>
    constexpr bool operator!=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(lhs == rhs);
    }

    template <class T>
    constexpr bool operator<(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return lhs.radians() < rhs.radians();
    }
    template <class T>
    constexpr bool operator<=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(rhs < lhs);
    }
    template <class T>
    constexpr bool operator>(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return rhs.radians() < lhs.radians();
    }
    template <class T>
    constexpr bool operator>=(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return !(lhs < rhs);
    }

    template <class T>
    constexpr Angle2<T> operator+(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return Angle2<T>::radians(lhs.radians() + rhs.radians());
    }
    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& lhs, const Angle2<T>& rhs)
    {
        return Angle2<T>::radians(lhs.radians() - rhs.radians());
    }
    template <class T>
    constexpr Angle2<T> operator*(const Angle2<T>& lhs, const T& rhs)
    {
        return Angle2<T>::radians(lhs.radians() * rhs);
    }
    template <class T>
    constexpr Angle2<T> operator/(const Angle2<T>& lhs, const T& rhs)
    {
        return Angle2<T>::radians(lhs.radians() / rhs);
    }

    template <class T>
    constexpr Angle2<T> operator-(const Angle2<T>& angle)
    {
        return Angle2<T>::radians(-angle.radians());
    }
}