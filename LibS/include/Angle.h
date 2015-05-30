#ifndef ANGLE_H
#define ANGLE_H

template <class T>
class Angle
{
public:
    Angle() = default;

    Angle(const Angle<T>& a) = default;
    template <class X>
    Angle(const Angle<X>& a);
    Angle(Angle<T>&& a) = default;

    static Angle<T> radians(T rad);
    static Angle<T> degrees(T deg);

    Angle<T>& operator=(const Angle<T>& a) = default;
    Angle<T>& operator=(Angle<T> && a) = default;
    template <class X>
    Angle<T>& operator=(const Angle<X>& a);

    Angle<T> operator+(const Angle<T>& a) const;
    Angle<T> operator-(const Angle<T>& a) const;
    Angle<T> operator*(const T scalar) const;
    Angle<T> operator/(const T scalar) const;

    Angle<T>& operator+=(const Angle<T>& a);
    Angle<T>& operator-=(const Angle<T>& a);
    Angle<T>& operator*=(const T scalar);
    Angle<T>& operator/=(const T scalar);

    T sin() const;
    T cos() const;
    T tan() const;
    T cot() const;

    T asin() const;
    T acos() const;
    T atan() const;
    T acot() const;

    T sinh() const;
    T cosh() const;
    T tanh() const;
    T coth() const;

    T asinh() const;
    T acosh() const;
    T atanh() const;
    T acoth() const;

    T radians() const;
    T degrees() const;
protected:
private:
    T m_radians;

    Angle(T rad);
};

typedef Angle<double> AngleD;
typedef Angle<float> AngleF;
typedef Angle<int> AngleI;

#include "../src/Angle.cpp"

#endif // ANGLE_H
