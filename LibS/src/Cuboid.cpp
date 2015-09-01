template <class T>
Cuboid<T>::Cuboid(const Vec3<T>& p1, const Vec3<T>& p2) : min(p1), max(p2)
{
}
template <class T>
Cuboid<T>::Cuboid(const Vec3<T>& p1, const T width, const T height, const T depth) : min(p1), max(p1 + Vec3<T>(width, height, depth))
{
}

template <class T>
Cuboid<T> Cuboid<T>::operator +(const Vec3<T>& v) const
{
    return Cuboid<T>(min + v, max + v);
}
template <class T>
Cuboid<T>& Cuboid<T>::operator +=(const Vec3<T>& v)
{
    min += v;
    max += v;
    return *this;
}
template <class T>
Cuboid<T> Cuboid<T>::operator -(const Vec3<T>& v) const
{
    return Cuboid<T>(min - v, max - v);
}
template <class T>
Cuboid<T>& Cuboid<T>::operator -=(const Vec3<T>& v)
{
    min -= v;
    max -= v;
    return *this;
}

template <class T>
template <class T2>
Cuboid<T>::operator Cuboid<T2>() const
{
    return Cuboid<T2>(static_cast<Vec3<T2>>(min), static_cast<Vec3<T2>>(max));
}

template <class T>
T Cuboid<T>::width() const
{
    return max.x - min.x;
}
template <class T>
T Cuboid<T>::height() const
{
    return max.y - min.y;
}
template <class T>
T Cuboid<T>::depth() const
{
    return max.z - min.z;
}

/* INTERSECTIONS */

template <class T>
template <class S>
bool Cuboid<T>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}
