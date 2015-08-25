template <class T>
Circle<T>::Circle(const Vec2<T>& p1, T r) : origin(p1), radius(r)
{
}
template <class T>
Circle<T> Circle<T>::operator +(const Vec2<T>& v) const
{
    return Circle<T>(origin + v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator +=(const Vec2<T>& v)
{
    origin += v;
    return *this;
}
template <class T>
Circle<T> Circle<T>::operator -(const Vec2<T>& v) const
{
    return Circle<T>(origin - v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator -=(const Vec2<T>& v)
{
    origin -= v;
    return *this;
}

template <class T>
template <class T2>
Circle<T>::operator Circle<T2>() const
{
    return Circle<T2>(static_cast<Vec2<T2>>(origin), static_cast<T2>(radius));
}

template <class T>
void Circle<T>::translate(const Vec2<T>& v)
{
    origin.translate(v);
}
template <class T>
void Circle<T>::scale(const Vec2<T>& s) //undefined when s.x != s.y
{
    radius *= s.x;
    origin.scale(s);
}
template <class T>
T Circle<T>::distanceTo(const Vec2<T>& v1) const
{
    return std::max(T(0.0), v1.distanceTo(origin) - radius);
}
template <class T>
Vec2<T> Circle<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(Intersections::intersection(point, *this)) return point;

    return (point - origin).normalized() * radius;
}

template <class T>
Vec2<T> Circle<T>::centerOfMass() const
{
    return origin;
}

template <class T>
T Circle<T>::area() const
{
    return radius*radius*PI;
}
