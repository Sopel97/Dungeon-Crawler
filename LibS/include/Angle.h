#ifndef ANGLE_H
#define ANGLE_H

template <class T>
class Angle
{
public:
    Angle() = default;

    Angle(const Angle<T>& a){m_radians = a.m_radians;}
    Angle(Angle<T>&& a){m_radians = std::move(a.m_radians);}

    static Angle<T> radians(T rad);
    static Angle<T> degrees(T deg);

    Angle<T>& operator=(const Angle<T>& a){m_radians = a.m_radians; return *this;}
    Angle<T>& operator=(Angle<T> && a){m_radians = std::move(a.m_radians); return *this;}

    Angle<T> operator+(const Angle<T>& a) const;
    Angle<T> operator-(const Angle<T>& a) const;
    Angle<T> operator*(const T scalar) const;
    Angle<T> operator/(const T scalar) const;

    Angle<T> operator-() const;

    Angle<T>& operator+=(const Angle<T>& a);
    Angle<T>& operator-=(const Angle<T>& a);
    Angle<T>& operator*=(const T scalar);
    Angle<T>& operator/=(const T scalar);

    template <class T2>
    explicit operator Angle<T2>() const;

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

extern template class Angle<double>;
extern template class Angle<float>;
extern template class Angle<int>;

#include "../src/Angle.cpp"

#endif // ANGLE_H
