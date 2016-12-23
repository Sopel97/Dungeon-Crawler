#pragma once

#include "..\Fwd.h"

#include "Shape2.h"
#include "Vec2.h"

namespace ls
{
    template <class T>
    class Circle2 : public Shape2<T>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        Vec2<T> origin;
        T radius;

        constexpr Circle2() = default;
        constexpr Circle2(const Vec2<T>& _origin, const T& _radius) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value && std::is_nothrow_copy_constructible<T>::value);

        constexpr Circle2(const Circle2<T>& other) = default;
        constexpr Circle2(Circle2<T>&& other) = default;

        Circle2<T>& operator =(Circle2<T> && other) & = default;
        Circle2<T>& operator =(const Circle2<T>& other) & = default;

        constexpr Circle2<T> translated(const Vec2<T>& displacement) const;
        Circle2<T>& translate(const Vec2<T>& displacement) &;

        Rectangle2<T> boundingBox() const;

        template <class T2>
        explicit operator Circle2<T2>() const;

        constexpr Vec2<T> centerOfMass() const;
        constexpr T area() const;
    };

    template <class T>
    constexpr bool operator==(const Circle2<T>& lhs, const Circle2<T>& rhs);
    template <class T>
    constexpr bool operator!=(const Circle2<T>& lhs, const Circle2<T>& rhs);

    using Circle2D = Circle2<double> ;
    using Circle2F = Circle2<float> ;

    extern template class Circle2<double>;
    extern template class Circle2<float>;
}

#include "../src/Circle2.cpp"
