#pragma once

#include "..\Fwd.h"

#include "Shape2.h"
#include "Vec2.h"

namespace ls
{
    template <class T>
    class LineSegment2 : public Shape2<T>
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        using ValueType = T;

        Vec2<T> begin;
        Vec2<T> end;

        constexpr LineSegment2() = default;
        constexpr LineSegment2(const Vec2<T>& _begin, const Vec2<T>& _end) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value);

        constexpr LineSegment2(const LineSegment2<T>& other) = default;
        constexpr LineSegment2(LineSegment2<T>&& other) = default;

        LineSegment2<T>& operator=(const LineSegment2<T>& other) & = default;
        LineSegment2<T>& operator=(LineSegment2<T> && other) & = default;

        constexpr LineSegment2<T> translated(const Vec2<T>& displacement) const;
        LineSegment2<T>& translate(const Vec2<T>& displacement) &;

        template <class T2>
        explicit operator LineSegment2<T2>() const;

        constexpr T length() const;

        constexpr Vec2<T> centerOfMass() const;
    };

    template <class T>
    constexpr bool operator== (const LineSegment2<T>& lhs, const LineSegment2<T>& rhs);
    template <class T>
    constexpr bool operator!= (const LineSegment2<T>& lhs, const LineSegment2<T>& rhs);

    using LineSegment2D = LineSegment2<double>;
    using LineSegment2F = LineSegment2<float>;

    extern template class LineSegment2<double>;
    extern template class LineSegment2<float>;


    template <class T>
    class ConstLineSegment2View //underlying points are immutable
    {
    public:
        using ValueType = T;

        static ConstLineSegment2View<T> ordered(const Vec2<T>* a, const Vec2<T>* b) noexcept;

        ConstLineSegment2View(const Vec2<T>* a, const Vec2<T>* b) noexcept;

        ConstLineSegment2View(const ConstLineSegment2View<T>& other) = default;
        ConstLineSegment2View(ConstLineSegment2View<T>&& other) = default;

        ConstLineSegment2View<T>& operator=(const ConstLineSegment2View<T>& other) & = default;
        ConstLineSegment2View<T>& operator=(ConstLineSegment2View<T>&& other) & = default;

        const Vec2<T>& begin() const;
        const Vec2<T>& end() const;

        const Vec2<T>* ptrBegin() const;
        const Vec2<T>* ptrEnd() const;

        void setPtrBegin(const Vec2<T>* newA) &;
        void setPtrEnd(const Vec2<T>* newB) &;

        void sort() &;

        LineSegment2<T> toLineSegment() const;
    private:
        std::array<const Vec2<T>*, 2u> m_v;
    };

    template <class T>
    bool operator==(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs);
    template <class T>
    bool operator!=(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs);
    template <class T>
    bool operator<(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator<=(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>=(const ConstLineSegment2View<T>& lhs, const ConstLineSegment2View<T>& rhs); //compares pointers lexicographically

    using ConstLineSegment2ViewD = ConstLineSegment2View<double>;
    using ConstLineSegment2ViewF = ConstLineSegment2View<float>;

    extern template class ConstLineSegment2View<double>;
    extern template class ConstLineSegment2View<float>;
}

#include "../src/LineSegment2.cpp"
