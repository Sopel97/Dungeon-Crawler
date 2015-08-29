#include "../include/Intersections.h"

template <class T>
void Vec3<T>::fill(T value)
{
    x = value;
    y = value;
    z = value;
}

template <class T>
Vec3<T>::Vec3(T _x, T _y, T _z) : x(_x), y(_y), z(_z)
{
}
template <class T>
Vec3<T>::Vec3(const std::initializer_list<T>& list)
{
    typename std::initializer_list<T>::iterator it = list.begin();
    x = *it;
    y = *(it + 1);
    z = *(it + 2);
}
template <class T>
Vec3<T> Vec3<T>::operator+(const Vec3<T>& v1) const
{
    return Vec3<T>(x + v1.x, y + v1.y, z + v1.z);
}
template <class T>
Vec3<T> Vec3<T>::operator-(const Vec3<T>& v1) const
{
    return Vec3<T>(x - v1.x, y - v1.y, z - v1.z);
}
template <class T>
Vec3<T> Vec3<T>::operator*(const T scalar) const
{
    return Vec3<T>(x * scalar, y * scalar, z * scalar);
}
template <class T>
Vec3<T> Vec3<T>::operator*(const Vec3<T>& v1) const
{
    return Vec3<T>(x * v1.x, y * v1.y, z * v1.z);
}
template <class T>
Vec3<T> Vec3<T>::operator/(const T scalar) const
{
    return Vec3<T>(x / scalar, y / scalar, z / scalar);
}

template <class T>
Vec3<T> Vec3<T>::operator-() const
{
    return Vec3<T>(-x, -y, -z);
}
template <class T>
Vec3<T>& Vec3<T>::operator+=(const Vec3<T>& v1)
{
    x += v1.x;
    y += v1.y;
    z += v1.z;
    return *this;
}
template <class T>
Vec3<T>& Vec3<T>::operator-=(const Vec3<T>& v1)
{
    x -= v1.x;
    y -= v1.y;
    z -= v1.z;
    return *this;
}
template <class T>
Vec3<T>& Vec3<T>::operator*=(const T scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return *this;
}
template <class T>
Vec3<T>& Vec3<T>::operator*=(const Vec3<T>& v1)
{
    x *= v1.x;
    y *= v1.y;
    z *= v1.z;
    return *this;
}
template <class T>
Vec3<T>& Vec3<T>::operator/=(const T scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    return *this;
}

template <class T>
template <class T2>
Vec3<T>::operator Vec3<T2>() const
{
    return Vec3<T2>(static_cast<T2>(x), static_cast<T2>(y), static_cast<T2>(z));
}

template <class T>
T& Vec3<T>::operator [](int i)
{
    return v[i];
}

template <class T>
const T& Vec3<T>::operator [](int i) const
{
    return v[i];
}

template <class T>
T Vec3<T>::magnitude()
{
    return static_cast<T>(std::sqrt((x * x) + (y * y) + (z * z)));
}
template <class T>
T Vec3<T>::distance(const Vec3<T>& v)
{
    T dx = x - v.x;
    T dy = y - v.y;
    T dz = z - v.z;
    return static_cast<T>(std::sqrt(dx * dx + dy * dy + dz * dz));
}
template <class T>
void Vec3<T>::normalize()
{
    T invMag = T(1) / magnitude();
    x *= invMag;
    y *= invMag;
    z *= invMag;
}
template <class T>
Vec3<T> Vec3<T>::normalized()
{
    T invMag = T(1) / magnitude();
    return Vec3<T>(x * invMag, y * invMag, z * invMag);
}
template <class T>
template <class Transformation>
void Vec3<T>::transform(Transformation&& func)
{
    func(*this);
}
template <class T>
template <class S>
bool Vec3<T>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}
