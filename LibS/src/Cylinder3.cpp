#include "..\include\Cylinder3.h"

namespace ls
{
    template <class T>
    Cylinder3<T>::Cylinder3(const Vec3<T>& p1, T r, T h) : center(p1), radius(r), height(h)
    {
    }
    template <class T>
    Cylinder3<T> Cylinder3<T>::translated(const Vec3<T>& v) const
    {
        return Cylinder3<T>(center + v, radius, height);
    }
    template <class T>
    Cylinder3<T>& Cylinder3<T>::translate(const Vec3<T>& v)
    {
        center += v;
        return *this;
    }

    template <class T>
    template <class T2>
    Cylinder3<T>::operator Cylinder3<T2>() const
    {
        return Cylinder3<T2>(static_cast<Vec3<T2>>(center), static_cast<T2>(radius), static_cast<T2>(height));
    }
    template <class T>
    Circle2<T> Cylinder3<T>::base() const
    {
        return Circle2<T>(Vec2<T>(center.x, center.y), radius);
    }
}