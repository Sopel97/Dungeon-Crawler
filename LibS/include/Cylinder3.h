#pragma once

#include "..\Fwd.h"

#include "Shape3.h"
#include "Vec3.h"
#include "Circle2.h"

namespace ls
{
    template <class T>
    class Cylinder3 : public Shape3<T>
    {
    public:
        using ValueType = T;

        Vec3<T> center; //cylinder base center
        T radius;
        T height;

        Cylinder3() = default;
        Cylinder3(const Vec3<T>& p1, T r, T h);

        Cylinder3(const Cylinder3<T>& c) { center = c.center; radius = c.radius; height = c.height; }
        Cylinder3(Cylinder3<T>&& c) { center = std::move(c.center); radius = std::move(c.radius); height = std::move(c.height); }

        Cylinder3<T>& operator =(const Cylinder3<T>& c) { center = c.center; radius = c.radius; height = c.height; return *this; }
        Cylinder3<T>& operator =(Cylinder3<T> && c) { center = std::move(c.center); radius = std::move(c.radius); height = std::move(c.height); return *this; }

        Cylinder3<T> translated(const Vec3<T>& v) const;
        Cylinder3<T>& translate(const Vec3<T>& v);

        template <class T2>
        explicit operator Cylinder3<T2>() const;

        Circle2<T> base() const;
    };

    using Cylinder3D = Cylinder3<double>;
    using Cylinder3F = Cylinder3<float>;

    extern template class Cylinder3<double>;
    extern template class Cylinder3<float>;
}

#include "../src/Cylinder3.cpp"
