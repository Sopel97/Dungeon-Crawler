#pragma once

#include "..\Fwd.h"

#include "Triangle2.h"
#include "Vec2.h"

namespace ls
{
    template <class T>
    class AffineTransformation2 : public Transformation2<T>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        T a, b, c; //matrix {{a,b},{c,d}}+vector{c,f}
        T d, e, f;

        constexpr AffineTransformation2() noexcept(std::is_nothrow_constructible<T, T>::value); //initialized to identity transform
        constexpr AffineTransformation2(const T& _a, const T& _b, const T& _c, const T& _d, const T& _e, const T& _f) noexcept(std::is_nothrow_copy_constructible<T>::value);
        constexpr AffineTransformation2(const AffineTransformation2<T>& other) = default;
        constexpr AffineTransformation2(AffineTransformation2<T>&& other) = default;

        AffineTransformation2<T>& operator=(const AffineTransformation2<T>& other) & = default;
        AffineTransformation2<T>& operator=(AffineTransformation2<T>&& other) & = default;

        static AffineTransformation2<T> fromTriangleMapping(const Triangle2<T>& from, const Triangle2<T>& to);
        static AffineTransformation2<T> identity() noexcept(std::is_nothrow_default_constructible<AffineTransformation2<T>>::value);

        AffineTransformation2<T>& rotateClockwise(const Angle2<T>& angle);
        AffineTransformation2<T>& rotateAntiClockwise(const Angle2<T>& angle);

        AffineTransformation2<T>& translate(const Vec2<T>& displacement);

        AffineTransformation2<T>& scale(const Vec2<T>& factor);
        AffineTransformation2<T>& scale(const T& factor);

        AffineTransformation2<T>& shearInXDirection(const T& factor);
        AffineTransformation2<T>& shearInYDirection(const T& factor);
        AffineTransformation2<T>& shear(const Vec2<T>& factor);

        AffineTransformation2<T>& reflectOverOrigin();
        AffineTransformation2<T>& reflectOverXAxis();
        AffineTransformation2<T>& reflectOverYAxis();

        virtual void transform(Vec2<T>& vector) const;
        virtual Vec2<T> transformed(const Vec2<T>& vector) const;

        AffineTransformation2<T> combined(const AffineTransformation2<T>& other) const;
        AffineTransformation2<T>& combine(const AffineTransformation2<T>& other);
    private:
    };

    template <class T>
    constexpr bool operator==(const AffineTransformation2<T>& lhs, const AffineTransformation2<T>& rhs);
    template <class T>
    constexpr bool operator!=(const AffineTransformation2<T>& lhs, const AffineTransformation2<T>& rhs);

    using AffineTransformation2D = AffineTransformation2<double>;
    using AffineTransformation2F = AffineTransformation2<float>;

    extern template class AffineTransformation2<double>;
    extern template class AffineTransformation2<float>;
}

#include "../src/AffineTransformation2.cpp"
