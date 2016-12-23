#pragma once

#include "..\Fwd.h"

#include "Shape3.h"
#include "Vec3.h"

namespace ls
{
    template <class T>
    class Cuboid3 : public Shape3<T>
    {
    public:
        using ValueType = T;

        Vec3<T> min, max;

        Cuboid3() = default;
        Cuboid3(const Vec3<T>& p1, const Vec3<T>& p2);
        Cuboid3(const Vec3<T>& p1, const T width, const T height, const T depth);

        Cuboid3(const Cuboid3<T>& c) { min = c.min; max = c.max; }
        Cuboid3(Cuboid3<T>&& c) { min = std::move(c.min); max = std::move(c.max); }

        Cuboid3<T>& operator =(const Cuboid3<T>& c) { min = c.min; max = c.max; return *this; }
        Cuboid3<T>& operator =(Cuboid3<T> && c) { min = c.min; max = c.max; return *this; }

        Cuboid3<T> translated(const Vec3<T>& v) const;
        Cuboid3<T>& translate(const Vec3<T>& v);

        template <class T2>
        explicit operator Cuboid3<T2>() const;

        T width() const;
        T height() const;
        T depth() const;
    };

    using Cuboid3D = Cuboid3<double>;
    using Cuboid3F = Cuboid3<float>;

    extern template class Cuboid3<double>;
    extern template class Cuboid3<float>;
}

#include "../src/Cuboid3.cpp"
