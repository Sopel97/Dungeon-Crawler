template <class T>
Cylinder<T>::Cylinder(const Vec3<T>& p1, T r, T h) : center(p1), radius(r), height(h)
{
}
template <class T>
template <class X>
Cylinder<T>::Cylinder(const Cylinder<X>& c) : center(c.p1), radius(c.r), height(c.h)
{
}

template <class T>
template <class X>
Cylinder<T>& Cylinder<T>::operator =(const Cylinder<X>& c)
{
    center = c.center;
    radius = c.radius;
    height = c.height;
    return *this;
}
template <class T>
Cylinder<T> Cylinder<T>::operator +(const Vec3<T>& v) const
{
    return Cylinder<T>(center + v, radius, height);
}
template <class T>
Cylinder<T>& Cylinder<T>::operator +=(const Vec3<T>& v)
{
    center += v;
    return *this;
}
template <class T>
Cylinder<T> Cylinder<T>::operator -(const Vec3<T>& v) const
{
    return Cylinder<T>(center - v, radius, height);
}
template <class T>
Cylinder<T>& Cylinder<T>::operator -=(const Vec3<T>& v)
{
    center -= v;
    return *this;
}
template <class T>
Circle<T> Cylinder<T>::base() const
{
    return Circle<T>(Vec2<T>(center.x, center.y), radius);
}
template <class T>
template <class Transformation>
void Cylinder<T>::transform(Transformation&& func)
{
    func(*this);
}

/* INTERSECTIONS */

template <class T>
template <class S>
bool Cylinder<T>::intersects(const S& b) const
{
    return Intersections::intersection(*this, b);
}
