#include "../include/Intersections.h"

template <class T>
Vec2<T> Vec2<T>::operator+(const Vec2<T>& v1) const
{
    return Vec2<T>(x + v1.x, y + v1.y);
}
template <class T>
Vec2<T> Vec2<T>::operator-(const Vec2<T>& v1) const
{
    return Vec2<T>(x - v1.x, y - v1.y);
}
template <class T>
Vec2<T> Vec2<T>::operator*(const T scalar) const
{
    return Vec2<T>(x * scalar, y * scalar);
}
template <class T>
Vec2<T> Vec2<T>::operator*(const Vec2<T>& v1) const
{
    return Vec2<T>(x * v1.x, y * v1.y);
}
template <class T>
Vec2<T> Vec2<T>::operator/(const T scalar) const
{
    return Vec2<T>(x / scalar, y / scalar);
}
template <class T>
Vec2<T> Vec2<T>::operator/(const Vec2<T>& v1) const
{
    return Vec2<T>(x / v1.x, y / v1.y);
}

template <class T>
Vec2<T> Vec2<T>::operator-() const
{
    return Vec2<T>(-x, -y);
}
template <class T>
T Vec2<T>::magnitude() const
{
    return std::sqrt(x * x + y * y);
}
template <class T>
T Vec2<T>::quadrance() const
{
    return x * x + y * y;
}
template <class T>
T Vec2<T>::distanceTo(const LineSegment<T>& lineSegment) const
{
    return distanceTo(lineSegment.nearestPointTo(*this));
}
template <class T>
void Vec2<T>::normalize()
{
    T invertedSquareRoot = T(1) / std::sqrt(x * x + y * y);
    x *= invertedSquareRoot;
    y *= invertedSquareRoot;
}
template <class T>
Vec2<T> Vec2<T>::normalized() const
{
    T invertedSquareRoot = T(1) / std::sqrt(x * x + y * y);
    return Vec2<T>(x * invertedSquareRoot, y * invertedSquareRoot);
}
template <class T>
T Vec2<T>::dot(const Vec2<T>& b) const
{
    return x * b.x + y * b.y;
}
template <class T>
T Vec2<T>::cross(const Vec2<T>& b) const
{
    return x * b.y - y * b.x;
}
template <class T>
Vec2<T> Vec2<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;
    T firstPart = dot(b) / (b.x * b.x + b.y * b.y);
    projection.x = firstPart * b.x;
    projection.y = firstPart * b.y;
    return projection;
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
Vec2<T> Vec2<T>::normal() const //right one
{
    return Vec2<T>(y, -x).normalized();
}
template <class T>
Angle<T> Vec2<T>::angle() const
{
    return Angle<T>::radians(atan2(y, x));
}
template <class T>
Angle<T> Vec2<T>::angle(const Vec2<T>& other) const
{
    return Angle<T>::radians(atan2(cross(other), dot(other)));
}


template <class T>
Vec2<T>::Vec2(T _xy) : x(_xy), y(_xy)
{
}
template <class T>
Vec2<T>::Vec2() : x(0), y(0)
{
};
template <class T>
Vec2<T>::Vec2(T _x, T _y) : x(_x), y(_y)
{
}
template <class T>
Vec2<T>::Vec2(const Vec2<T>& v) : x(v.x), y(v.y)
{

}
template <class T>
Vec2<T>::Vec2(Vec2<T>&& v) : x(std::move(v.x)), y(std::move(v.y))
{

}
template <class T>
Vec2<T>::Vec2(const std::initializer_list<T>& list)
{
    typename std::initializer_list<T>::iterator it = list.begin();
    x = *it;
    y = *(it + 1);
}
template <class T>
Vec2<T>& Vec2<T>::operator=(const Vec2<T>& v1)
{
    x = v1.x;
    y = v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator=(Vec2<T> && v1)
{
    x = std::move(v1.x);
    y = std::move(v1.y);
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator+=(const Vec2<T>& v1)
{
    x += v1.x;
    y += v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator-=(const Vec2<T>& v1)
{
    x -= v1.x;
    y -= v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator*=(const T scalar)
{
    x *= scalar;
    y *= scalar;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator*=(const Vec2<T>& v1)
{
    x *= v1.x;
    y *= v1.y;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator/=(const T scalar)
{
    x /= scalar;
    y /= scalar;
    return *this;
}
template <class T>
Vec2<T>& Vec2<T>::operator/=(const Vec2<T>& v1)
{
    x /= v1.x;
    y /= v1.y;
    return *this;
}

template <class T>
template <class T2>
Vec2<T>::operator Vec2<T2>() const
{
    return Vec2<T2>(static_cast<T2>(x), static_cast<T2>(y));
}
template <class T>
T& Vec2<T>::operator[](size_t index)
{
    return v[index];
}
template <class T>
const T& Vec2<T>::operator[](size_t index) const
{
    return v[index];
}

template <class T>
void Vec2<T>::translate(const Vec2<T>& v)
{
    x += v.x;
    y += v.y;
}
template <class T>
void Vec2<T>::scale(const Vec2<T>& c, const Vec2<T>& s)
{
    x = (x - c.x) * s.x + c.x;
    y = (y - c.y) * s.y + c.y;
}
template <class T>
void Vec2<T>::scale(const Vec2<T>& c, const T s)
{
    x = (x - c.x) * s + c.x;
    y = (y - c.y) * s + c.y;
}
template <class T>
void Vec2<T>::scale(const Vec2<T>& s)
{
    x *= s.x;
    y *= s.y;
}
template <class T>
void Vec2<T>::scale(const T s)
{
    x *= s;
    y *= s;
}
template <class T>
T Vec2<T>::distanceTo(const Vec2<T>& v1) const
{
    T dx = x - v1.x;
    T dy = y - v1.y;
    return std::sqrt(dx * dx + dy * dy);
}
template <class T>
Vec2<T> Vec2<T>::nearestPointTo(const Vec2<T>& v1) const
{
    return *this;
}

template <class T>
Vec2<T> Vec2<T>::direction(const Angle<T>& angle)
{
    return Vec2<T>(angle.cos(), angle.sin());
}
