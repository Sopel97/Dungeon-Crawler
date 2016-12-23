#pragma once

#include "..\Fwd.h"

#include "Shape2.h"
#include "Vec2.h"
#include "Polyline2.h"

#include <array>

namespace ls
{
    template <class T>
    class Triangle2 : public Shape2<T>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        std::array<Vec2<T>, 3u> vertices;

        constexpr Triangle2() = default;
        constexpr Triangle2(const Vec2<T>& v1, const Vec2<T>& v2, const Vec2<T>& v3) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value);

        constexpr static Triangle2<T> equilateral(const Vec2<T>& center, const T& baseLength) noexcept(std::is_nothrow_constructible<Triangle2<T>, const Vec2<T>&, const Vec2<T>&, const Vec2<T>&>::value);
        constexpr static Triangle2<T> isosceles(const Vec2<T>& center, const T& baseLength, const T& height) noexcept(std::is_nothrow_constructible<Triangle2<T>, const Vec2<T>&, const Vec2<T>&, const Vec2<T>&>::value);
        constexpr static Triangle2<T> right(const Vec2<T>& rightAngledVertex, const T& width, const T& height) noexcept(std::is_nothrow_constructible<Triangle2<T>, const Vec2<T>&, const Vec2<T>&, const Vec2<T>&>::value); //with 90 degree angle on the bottom left

        constexpr Triangle2(const Triangle2<T>& other) = default;
        constexpr Triangle2(Triangle2<T>&& other) = default;

        Triangle2<T>& operator =(const Triangle2<T>& other) & = default;
        Triangle2<T>& operator =(Triangle2<T> && other) & = default;

        constexpr Triangle2<T> translated(const Vec2<T>& displacement) const;
        Triangle2<T>& translate(const Vec2<T>& displacement) &;

        template <class T2>
        explicit operator Triangle2<T2>() const;

        constexpr Vec2<T> centerOfMass() const;
        constexpr T signedArea() const;
        constexpr T area() const;
        Vec2<T> circumcenter() const;
        Circle2<T> circumcircle() const;
    };

    extern template class Triangle2<double>;
    extern template class Triangle2<float>;

    using Triangle2D = Triangle2<double>;
    using Triangle2F = Triangle2<float>;



    template <class T>
    class ConstTriangle2View //underlying vertices are immutable
    {
    public:
        using ValueType = T;

        static ConstTriangle2View<T> ordered(const Vec2<T>* a, const Vec2<T>* b, const Vec2<T>* c) noexcept;

        ConstTriangle2View(const Vec2<T>* a, const Vec2<T>* b, const Vec2<T>* c) noexcept;

        ConstTriangle2View(const ConstTriangle2View<T>& other) = default;
        ConstTriangle2View(ConstTriangle2View<T>&& other) = default;

        ConstTriangle2View<T>& operator=(const ConstTriangle2View<T>& other) & = default;
        ConstTriangle2View<T>& operator=(ConstTriangle2View<T>&& other) & = default;

        const Vec2<T>& a() const;
        const Vec2<T>& b() const;
        const Vec2<T>& c() const;

        const Vec2<T>* ptrToA() const;
        const Vec2<T>* ptrToB() const;
        const Vec2<T>* ptrToC() const;

        void setPtrToA(const Vec2<T>* newPtrToA) &;
        void setPtrToB(const Vec2<T>* newPtrToB) &;
        void setPtrToC(const Vec2<T>* newPtrToC) &;

        void sort() &;

        Triangle2<T> toTriangle() const;

    private:
        std::array<const Vec2<T>*, 3u> m_v;
    };

    template <class T>
    bool operator==(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs);
    template <class T>
    bool operator!=(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs);
    template <class T>
    bool operator<(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator<=(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>=(const ConstTriangle2View<T>& lhs, const ConstTriangle2View<T>& rhs); //compares pointers lexicographically

    using ConstTriangle2ViewD = ConstTriangle2View<double>;
    using ConstTriangle2ViewF = ConstTriangle2View<float>;

    extern template class ConstTriangle2View<double>;
    extern template class ConstTriangle2View<float>;

}
#include "../src/Triangle2.cpp"
