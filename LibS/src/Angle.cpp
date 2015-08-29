template <class T>
Angle<T>::Angle(T rad) : m_radians(rad)
{

}

template <class T>
Angle<T> Angle<T>::operator+(const Angle<T>& a) const
{
    return Angle<T>::radians(m_radians + a.m_radians);
}
template <class T>
Angle<T> Angle<T>::operator-(const Angle<T>& a) const
{
    return Angle<T>::radians(m_radians - a.m_radians);
}
template <class T>
Angle<T> Angle<T>::operator*(const T scalar) const
{
    return Angle<T>::radians(m_radians * scalar);
}
template <class T>
Angle<T> Angle<T>::operator/(const T scalar) const
{
    return Angle<T>::radians(m_radians / scalar);
}

template <class T>
Angle<T>& Angle<T>::operator+=(const Angle<T>& a)
{
    m_radians += a.m_radians;
    return *this;
}
template <class T>
Angle<T>& Angle<T>::operator-=(const Angle<T>& a)
{
    m_radians -= a.m_radians;
    return *this;
}
template <class T>
Angle<T>& Angle<T>::operator*=(const T scalar)
{
    m_radians *= scalar;
    return *this;
}
template <class T>
Angle<T>& Angle<T>::operator/=(const T scalar)
{
    m_radians /= scalar;
    return *this;
}

template <class T>
Angle<T> Angle<T>::operator-() const
{
    return radians(-m_radians);
}
template <class T>
template <class T2>
Angle<T>::operator Angle<T2>() const
{
    return Angle<T2>::radians(static_cast<T2>(m_radians));
}

template <class T>
T Angle<T>::sin() const
{
    return static_cast<T>(std::sin(m_radians));
}
template <class T>
T Angle<T>::cos() const
{
    return static_cast<T>(std::cos(m_radians));
}
template <class T>
T Angle<T>::tan() const
{
    return static_cast<T>(std::tan(m_radians));
}
template <class T>
T Angle<T>::cot() const
{
    return T(1) / static_cast<T>(std::tan(m_radians));
}

template <class T>
T Angle<T>::asin() const
{
    return static_cast<T>(std::asin(m_radians));
}
template <class T>
T Angle<T>::acos() const
{
    return static_cast<T>(std::acos(m_radians));
}
template <class T>
T Angle<T>::atan() const
{
    return static_cast<T>(std::atan(m_radians));
}
template <class T>
T Angle<T>::acot() const
{
    return (static_cast<T>(PI) / T(2)) - static_cast<T>(std::atan(m_radians));
}

template <class T>
T Angle<T>::sinh() const
{
    return static_cast<T>(std::sinh(m_radians));
}
template <class T>
T Angle<T>::cosh() const
{
    return static_cast<T>(std::cosh(m_radians));
}
template <class T>
T Angle<T>::tanh() const
{
    return static_cast<T>(std::tanh(m_radians));
}
template <class T>
T Angle<T>::coth() const
{
    return T(1) / static_cast<T>(std::tanh(m_radians));
}

template <class T>
T Angle<T>::asinh() const
{
    return static_cast<T>(std::atanh(m_radians));
}
template <class T>
T Angle<T>::acosh() const
{
    return static_cast<T>(std::acosh(m_radians));
}
template <class T>
T Angle<T>::atanh() const
{
    return static_cast<T>(std::atanh(m_radians));
}
template <class T>
T Angle<T>::acoth() const
{
    return static_cast<T>(std::atanh(T(1) / m_radians));
}

template <class T>
T Angle<T>::radians() const
{
    return m_radians;
}
template <class T>
T Angle<T>::degrees() const
{
    return radiansToDegrees(m_radians);
}

template <class T>
Angle<T> Angle<T>::radians(T rad)
{
    return Angle<T>(rad);
}
template <class T>
Angle<T> Angle<T>::degrees(T deg)
{
    return Angle<T>(degreesToRadians(deg));
}
