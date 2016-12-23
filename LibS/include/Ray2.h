#pragma once

#include "..\Fwd.h"

#include "Shape2.h"
#include "Vec2.h"

namespace ls
{
    template <class T>
    class Ray2 : public Shape2<T>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        Vec2<T> origin;
        Vec2<T> direction;

        constexpr Ray2() = default;
        constexpr Ray2(const Vec2<T>& _origin, const Vec2<T>& _direction) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value);

        constexpr Ray2(const Ray2<T>& other) = default;
        constexpr Ray2(Ray2<T>&& other) = default;

        Ray2<T>& operator=(const Ray2<T>& other) & = default;
        Ray2<T>& operator=(Ray2<T> && other) & = default;

        constexpr Ray2<T> translated(const Vec2<T>& displacement) const;
        Ray2<T>& translate(const Vec2<T>& displacement) &;

        template <class T2>
        explicit operator Ray2<T2>() const;
    };

    template <class T>
    constexpr bool operator== (const Ray2<T>& lhs, const Ray2<T>& rhs);
    template <class T>
    constexpr bool operator!= (const Ray2<T>& lhs, const Ray2<T>& rhs);

    using Ray2D = Ray2<double>;
    using Ray2F = Ray2<float>;

    extern template class Ray2<double>;
    extern template class Ray2<float>;
}
#include "../src/Ray2.cpp"
