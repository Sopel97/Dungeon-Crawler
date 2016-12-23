#include "..\include\Cuboid3.h"

namespace ls
{
    template <class T>
    Cuboid3<T>::Cuboid3(const Vec3<T>& p1, const Vec3<T>& p2) : min(p1), max(p2)
    {
    }
    template <class T>
    Cuboid3<T>::Cuboid3(const Vec3<T>& p1, const T width, const T height, const T depth) : min(p1), max(p1 + Vec3<T>(width, height, depth))
    {
    }

    template <class T>
    Cuboid3<T> Cuboid3<T>::translated(const Vec3<T>& v) const
    {
        return Cuboid3<T>(min + v, max + v);
    }
    template <class T>
    Cuboid3<T>& Cuboid3<T>::translate(const Vec3<T>& v)
    {
        min += v;
        max += v;
        return *this;
    }

    template <class T>
    template <class T2>
    Cuboid3<T>::operator Cuboid3<T2>() const
    {
        return Cuboid3<T2>(static_cast<Vec3<T2>>(min), static_cast<Vec3<T2>>(max));
    }

    template <class T>
    T Cuboid3<T>::width() const
    {
        return max.x - min.x;
    }
    template <class T>
    T Cuboid3<T>::height() const
    {
        return max.y - min.y;
    }
    template <class T>
    T Cuboid3<T>::depth() const
    {
        return max.z - min.z;
    }
}