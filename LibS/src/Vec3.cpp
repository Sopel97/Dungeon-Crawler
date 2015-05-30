#include "../include/Intersections.h"

/* proxy */

template <class T, size_t X, size_t Y, size_t Z>
void Vec3Proxy<T, X, Y, Z>::fill(T value)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = value;
    getY() = value;
    getZ() = value;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::operator+(const Vec3<T>& v1) const
{
    return Vec3<T>(getX() + v1.x, getY() + v1.y, getZ() + v1.z);
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::operator-(const Vec3<T>& v1) const
{
    return Vec3<T>(getX() - v1.x, getY() - v1.y, getZ() - v1.z);
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::operator*(const T scalar) const
{
    return Vec3<T>(getX() * scalar, getY() * scalar, getZ() * scalar);
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::operator*(const Vec3<T>& v1) const
{
    return Vec3<T>(getX() * v1.x, getY() * v1.y, getZ() * v1.z);
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::operator/(const T scalar) const
{
    return Vec3<T>(getX() / scalar, getY() / scalar, getZ() / scalar);
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::operator/(const Vec3<T>& v1) const
{
    return Vec3<T>(getX() / v1.x, getY() / v1.y, getZ() / v1.z);
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::operator-() const
{
    return Vec3<T>(-getX(), -getY(), -getZ());
}

template <class T, size_t X, size_t Y, size_t Z>
T Vec3Proxy<T, X, Y, Z>::magnitude() const
{
    return std::sqrt(getX() * getX() + getY() * getY() + getZ() * getZ());
}
template <class T, size_t X, size_t Y, size_t Z>
T Vec3Proxy<T, X, Y, Z>::distanceTo(const Vec3<T>& v1) const
{
    T dx = getX() - v1.x;
    T dy = getY() - v1.y;
    T dz = getZ() - v1.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
template <class T, size_t X, size_t Y, size_t Z>
void Vec3Proxy<T, X, Y, Z>::normalize()
{
    T invertedMagnitude = 1. / magnitude();
    getX() *= invertedMagnitude;
    getY() *= invertedMagnitude;
    getZ() *= invertedMagnitude;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3<T> Vec3Proxy<T, X, Y, Z>::normalized() const
{
    T invertedMagnitude = 1. / magnitude();
    return Vec3<T>(getX() * invertedMagnitude, getY() * invertedMagnitude, getZ() * invertedMagnitude);
}

template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator=(const Vec3<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = v1.x;
    getY() = v1.y;
    getZ() = v1.z;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator=(Vec3<T> && v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = std::move(v1.x);
    getY() = std::move(v1.y);
    getZ() = std::move(v1.z);
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
template <class U>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator=(const Vec3<U>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = v1.x;
    getY() = v1.y;
    getZ() = v1.z;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator+=(const Vec3<T>& v1)
{
    getX() += v1.x;
    getY() += v1.y;
    getZ() += v1.z;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator-=(const Vec3<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() -= v1.x;
    getY() -= v1.y;
    getZ() -= v1.z;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator*=(const T scalar)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() *= scalar;
    getY() *= scalar;
    getZ() *= scalar;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator*=(const Vec3<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() *= v1.x;
    getY() *= v1.y;
    getZ() *= v1.y;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator/=(const T scalar)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() /= scalar;
    getY() /= scalar;
    getZ() /= scalar;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>& Vec3Proxy<T, X, Y, Z>::operator/=(const Vec3<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() /= v1.x;
    getY() /= v1.y;
    getZ() /= v1.y;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z>
T& Vec3Proxy<T, X, Y, Z>::operator[](size_t index)
{
    switch(index)
    {
        case 0:
            return getX();
            break;
        case 1:
            return getY();
            break;
        case 2:
            return getZ();
            break;
    }
    return getX();
}
template <class T, size_t X, size_t Y, size_t Z>
const T& Vec3Proxy<T, X, Y, Z>::operator[](size_t index) const
{
    switch(index)
    {
        case 0:
            return getX();
            break;
        case 1:
            return getY();
            break;
        case 2:
            return getZ();
            break;
    }
    return getX();
}

template <class T, size_t X, size_t Y, size_t Z>
Vec3Proxy<T, X, Y, Z>::operator Vec3<T>() const
{
    return Vec3<T>(getX(), getY(), getZ());
}
template <class T, size_t X, size_t Y, size_t Z>
T& Vec3Proxy<T, X, Y, Z>::getX()
{
    return *(((T*)this) + X);
}
template <class T, size_t X, size_t Y, size_t Z>
const T& Vec3Proxy<T, X, Y, Z>::getX() const
{
    return *(((T*)this) + X);
}
template <class T, size_t X, size_t Y, size_t Z>
T& Vec3Proxy<T, X, Y, Z>::getY()
{
    return *(((T*)this) + Y);
}
template <class T, size_t X, size_t Y, size_t Z>
const T& Vec3Proxy<T, X, Y, Z>::getY() const
{
    return *(((T*)this) + Y);
}
template <class T, size_t X, size_t Y, size_t Z>
T& Vec3Proxy<T, X, Y, Z>::getZ()
{
    return *(((T*)this) + Z);
}
template <class T, size_t X, size_t Y, size_t Z>
const T& Vec3Proxy<T, X, Y, Z>::getZ() const
{
    return *(((T*)this) + Z);
}

/* normal */

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
template <class X>
Vec3<T>::Vec3(const Vec3<X>& v) : x(v.x), y(v.y), z(v.z)
{
}
template <class T>
template <class X>
Vec3<T>& Vec3<T>::operator=(const Vec3<X>& v1)
{
    x = v1.x;
    y = v1.y;
    z = v1.z;
    return *this;
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
    return std::sqrt((x * x) + (y * y) + (z * z));
}
template <class T>
T Vec3<T>::distance(const Vec3<T>& v)
{
    T dx = x - v.x;
    T dy = y - v.y;
    T dz = z - v.z;
    return std::sqrt(dx * dx + dy * dy + dz * dz);
}
template <class T>
void Vec3<T>::normalize()
{
    T invMag = 1.0 / magnitude();
    x *= invMag;
    y *= invMag;
    z *= invMag;
}
template <class T>
Vec3<T> Vec3<T>::normalized()
{
    T invMag = 1.0 / magnitude();
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
