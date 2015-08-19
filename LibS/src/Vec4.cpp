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
