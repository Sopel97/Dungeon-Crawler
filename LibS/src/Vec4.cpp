
/* proxy */

template <class T, size_t X, size_t Y, size_t Z, size_t W>
void Vec4Proxy<T, X, Y, Z, W>::fill(T value)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = value;
    getY() = value;
    getZ() = value;
    getW() = value;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::operator+(const Vec4<T>& v1) const
{
    return Vec4<T>(getX() + v1.x, getY() + v1.y, getZ() + v1.z, getW() + v1.w);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::operator-(const Vec4<T>& v1) const
{
    return Vec4<T>(getX() - v1.x, getY() - v1.y, getZ() - v1.z, getW() - v1.w);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::operator*(const T scalar) const
{
    return Vec4<T>(getX() * scalar, getY() * scalar, getZ() * scalar, getW() * scalar);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::operator*(const Vec4<T>& v1) const
{
    return Vec4<T>(getX() * v1.x, getY() * v1.y, getZ() * v1.z, getW() * v1.w);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::operator/(const T scalar) const
{
    return Vec4<T>(getX() / scalar, getY() / scalar, getZ() / scalar, getW() / scalar);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::operator/(const Vec4<T>& v1) const
{
    return Vec4<T>(getX() / v1.x, getY() / v1.y, getZ() / v1.z, getW() / v1.w);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::operator-() const
{
    return Vec4<T>(-getX(), -getY(), -getZ(), -getZ());
}

template <class T, size_t X, size_t Y, size_t Z, size_t W>
T Vec4Proxy<T, X, Y, Z, W>::magnitude() const
{
    return std::sqrt(getX() * getX() + getY() * getY() + getZ() * getZ() + getW() * getW());
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
T Vec4Proxy<T, X, Y, Z, W>::distanceTo(const Vec4<T>& v1) const
{
    T dx = getX() - v1.x;
    T dy = getY() - v1.y;
    T dz = getZ() - v1.z;
    T dw = getW() - v1.w;
    return std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
void Vec4Proxy<T, X, Y, Z, W>::normalize()
{
    T invertedMagnitude = 1. / magnitude();
    getX() *= invertedMagnitude;
    getY() *= invertedMagnitude;
    getZ() *= invertedMagnitude;
    getW() *= invertedMagnitude;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4<T> Vec4Proxy<T, X, Y, Z, W>::normalized() const
{
    T invertedMagnitude = 1. / magnitude();
    return Vec4<T>(getX() * invertedMagnitude, getY() * invertedMagnitude, getZ() * invertedMagnitude, getW() * invertedMagnitude);
}

template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator=(const Vec4<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = v1.x;
    getY() = v1.y;
    getZ() = v1.z;
    getW() = v1.w;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator=(Vec4<T> && v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = std::move(v1.x);
    getY() = std::move(v1.y);
    getZ() = std::move(v1.z);
    getW() = std::move(v1.w);
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
template <class U>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator=(const Vec4<U>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() = v1.x;
    getY() = v1.y;
    getZ() = v1.z;
    getW() = v1.w;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator+=(const Vec4<T>& v1)
{
    getX() += v1.x;
    getY() += v1.y;
    getZ() += v1.z;
    getW() += v1.w;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator-=(const Vec4<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() -= v1.x;
    getY() -= v1.y;
    getZ() -= v1.z;
    getW() -= v1.w;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator*=(const T scalar)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() *= scalar;
    getY() *= scalar;
    getZ() *= scalar;
    getW() *= scalar;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator*=(const Vec4<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() *= v1.x;
    getY() *= v1.y;
    getZ() *= v1.y;
    getW() *= v1.w;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator/=(const T scalar)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() /= scalar;
    getY() /= scalar;
    getZ() /= scalar;
    getW() /= scalar;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>& Vec4Proxy<T, X, Y, Z, W>::operator/=(const Vec4<T>& v1)
{
    static_assert(isWritable, "Can't perform modifying operation on vector through proxy with ununique references.");
    getX() /= v1.x;
    getY() /= v1.y;
    getZ() /= v1.y;
    getW() /= v1.w;
    return *this;
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
T& Vec4Proxy<T, X, Y, Z, W>::operator[](size_t index)
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
        case 3:
            return getW();
            break;
    }
    return getX();
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
const T& Vec4Proxy<T, X, Y, Z, W>::operator[](size_t index) const
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
        case 3:
            return getW();
            break;
    }
    return getX();
}

template <class T, size_t X, size_t Y, size_t Z, size_t W>
Vec4Proxy<T, X, Y, Z, W>::operator Vec4<T>() const
{
    return Vec4<T>(getX(), getY(), getZ(), getW());
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
T& Vec4Proxy<T, X, Y, Z, W>::getX()
{
    return *(((T*)this) + X);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
const T& Vec4Proxy<T, X, Y, Z, W>::getX() const
{
    return *(((T*)this) + X);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
T& Vec4Proxy<T, X, Y, Z, W>::getY()
{
    return *(((T*)this) + Y);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
const T& Vec4Proxy<T, X, Y, Z, W>::getY() const
{
    return *(((T*)this) + Y);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
T& Vec4Proxy<T, X, Y, Z, W>::getZ()
{
    return *(((T*)this) + Z);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
const T& Vec4Proxy<T, X, Y, Z, W>::getZ() const
{
    return *(((T*)this) + Z);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
T& Vec4Proxy<T, X, Y, Z, W>::getW()
{
    return *(((T*)this) + W);
}
template <class T, size_t X, size_t Y, size_t Z, size_t W>
const T& Vec4Proxy<T, X, Y, Z, W>::getW() const
{
    return *(((T*)this) + W);
}

/* normal */

template <class T>
void Vec4<T>::fill(T value)
{
    x = value;
    y = value;
    z = value;
    w = value;
}

template <class T>
Vec4<T>::Vec4(T _x, T _y, T _z, T _w) : x(_x), y(_y), z(_z), w(_w)
{
}
template <class T>
Vec4<T>::Vec4(const std::initializer_list<T>& list)
{
    typename std::initializer_list<T>::iterator it = list.begin();
    x = *it;
    y = *(it + 1);
    z = *(it + 2);
    w = *(it + 3);
}
template <class T>
template <class X>
Vec4<T>::Vec4(const Vec4<X>& v) : x(v.x), y(v.y), z(v.z), w(v.w)
{
}
template <class T>
template <class X>
Vec4<T>& Vec4<T>::operator=(const Vec4<X>& v1)
{
    x = v1.x;
    y = v1.y;
    z = v1.z;
    w = v1.w;
    return *this;
}
template <class T>
Vec4<T> Vec4<T>::operator+(const Vec4<T>& v1) const
{
    return Vec4<T>(x + v1.x, y + v1.y, z + v1.z, w + v1.w);
}
template <class T>
Vec4<T> Vec4<T>::operator-(const Vec4<T>& v1) const
{
    return Vec4<T>(x - v1.x, y - v1.y, z - v1.z, w - v1.w);
}
template <class T>
Vec4<T> Vec4<T>::operator*(const T scalar) const
{
    return Vec4<T>(x * scalar, y * scalar, z * scalar, w * scalar);
}
template <class T>
Vec4<T> Vec4<T>::operator*(const Vec4<T>& v1) const
{
    return Vec4<T>(x * v1.x, y * v1.y, z * v1.z, w * v1.w);
}
template <class T>
Vec4<T> Vec4<T>::operator/(const T scalar) const
{
    return Vec4<T>(x / scalar, y / scalar, z / scalar, w / scalar);
}
template <class T>
Vec4<T> Vec4<T>::operator-() const
{
    return Vec4<T>(-x, -y, -z, -w);
}

template <class T>
Vec4<T>& Vec4<T>::operator+=(const Vec4<T>& v1)
{
    x += v1.x;
    y += v1.y;
    z += v1.z;
    w += v1.w;
    return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator-=(const Vec4<T>& v1)
{
    x -= v1.x;
    y -= v1.y;
    z -= v1.z;
    w -= v1.w;
    return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator*=(const T scalar)
{
    x *= scalar;
    y *= scalar;
    z *= scalar;
    w *= scalar;
    return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator*=(const Vec4<T>& v1)
{
    x *= v1.x;
    y *= v1.y;
    z *= v1.z;
    w *= v1.w;
    return *this;
}
template <class T>
Vec4<T>& Vec4<T>::operator/=(const T scalar)
{
    x /= scalar;
    y /= scalar;
    z /= scalar;
    w /= scalar;
    return *this;
}

template <class T>
T& Vec4<T>::operator [](int i)
{
    return v[i];
}

template <class T>
const T& Vec4<T>::operator [](int i) const
{
    return v[i];
}

template <class T>
T Vec4<T>::magnitude()
{
    return std::sqrt((x * x) + (y * y) + (z * z) + (w * w));
}
template <class T>
T Vec4<T>::distance(const Vec4<T>& v)
{
    T dx = x - v.x;
    T dy = y - v.y;
    T dz = z - v.z;
    T dw = w - v.w;
    return std::sqrt(dx * dx + dy * dy + dz * dz + dw * dw);
}
template <class T>
void Vec4<T>::normalize()
{
    T invMag = 1.0 / magnitude();
    x *= invMag;
    y *= invMag;
    z *= invMag;
    w *= invMag;
}
template <class T>
Vec4<T> Vec4<T>::normalized()
{
    T invMag = 1.0 / magnitude();
    return Vec4<T>(x * invMag, y * invMag, z * invMag, w * invMag);
}
template <class T>
template <class Transformation>
void Vec4<T>::transform(Transformation&& func)
{
    func(*this);
}
