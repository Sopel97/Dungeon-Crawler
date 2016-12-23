#pragma once

#include "..\Fwd.h"

#include "Shape2.h"
#include "Vec2.h"
#include "Polyline2.h"

namespace ls
{
    template <class T>
    class Rectangle2 : public Shape2<T>
    {
        static_assert(std::is_arithmetic<T>::value, "T must be an arithmetic type");
    public:
        using ValueType = T;

        Vec2<T> min, max;

        constexpr Rectangle2() = default;
        constexpr Rectangle2(const Vec2<T>& _min, const Vec2<T>& _max) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value);

        static Rectangle2<T> withSize(const Vec2<T>& min, const T& width, const T& height) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value);
        static Rectangle2<T> unitRectangle() noexcept(std::is_nothrow_constructible<Rectangle2<T>, const Vec2<T>&, const Vec2<T>&>::value);

        constexpr Rectangle2(const Rectangle2<T>& other) = default;
        constexpr Rectangle2(Rectangle2<T>&& other) = default;

        Rectangle2<T>& operator =(const Rectangle2<T>& other) & = default;
        Rectangle2<T>& operator =(Rectangle2<T> && other) & = default;

        constexpr Rectangle2<T> translated(const Vec2<T>& displacement) const;
        Rectangle2<T>& translate(const Vec2<T>& displacement) &;

        template <class T2>
        explicit operator Rectangle2<T2>() const;

        constexpr T width() const;
        constexpr T height() const;

        constexpr Vec2<T> centerOfMass() const;
        constexpr T area() const;
    };

    template <class T>
    constexpr bool operator== (const Rectangle2<T>& lhs, const Rectangle2<T>& rhs);
    template <class T>
    constexpr bool operator!= (const Rectangle2<T>& lhs, const Rectangle2<T>& rhs);

    using Rectangle2D = Rectangle2<double>;
    using Rectangle2F = Rectangle2<float>;
    using Rectangle2I = Rectangle2<int>;

    extern template class Rectangle2<double>;
    extern template class Rectangle2<float>;
    extern template class Rectangle2<int>;
}
#include "../src/Rectangle2.cpp"
