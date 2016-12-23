#pragma once

#include "..\Fwd.h"

#include "Shape2.h"
#include "Vec2.h"
#include "Rectangle2.h"
#include "Triangle2.h"

#include <vector>
#include <initializer_list>

namespace ls
{
    template <class T>
    class Polyline2 : public Shape2<T>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        Polyline2() = default;
        explicit Polyline2(std::initializer_list<Vec2<T>> vertices);
        explicit Polyline2(const std::vector<Vec2<T>>& vertices);
        explicit Polyline2(std::vector<Vec2<T>>&& vertices) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value);
        template <class InputIt>
        Polyline2(InputIt begin, InputIt end);

        static Polyline2<T> fromLineSegment(const LineSegment2<T>& lineSegment);

        Polyline2(const Polyline2<T>& other) = default;
        Polyline2(Polyline2<T>&& other) = default;

        Polyline2<T>& operator=(const Polyline2<T>& other) & = default;
        Polyline2<T>& operator=(Polyline2<T> && other) & = default;

        Polyline2<T> translated(const Vec2<T>& displacement) const;
        Polyline2<T>& translate(const Vec2<T>& displacement) &;

        template <class T2>
        explicit operator Polyline2<T2>() const;

        template <class... Args>
        int emplaceVertex(Args&&... coords);
        void addVertex(const Vec2<T>& newVertex) &;
        template <class InputIt>
        void addVertices(InputIt begin, InputIt end) &;
        void addVertices(std::initializer_list<Vec2<T>> newVertices) &;

        const std::vector<Vec2<T>>& vertices() const &;
        std::vector<Vec2<T>> vertices() &&;
        const Vec2<T>& vertex(int i) const;
        Vec2<T>& vertex(int i);

        int size() const;

    protected:
        std::vector<Vec2<T>> m_vertices;
    };

    using Polyline2D = Polyline2<double>;
    using Polyline2F = Polyline2<float>;

    extern template class Polyline2<double>;
    extern template class Polyline2<float>;


    template <class T>
    class ConstPolyline2View //underlying points are immutable
    {
    public:
        using ValueType = T;

        static ConstPolyline2View<T> ordered(const std::vector<const Vec2<T>*>& vertices);
        static ConstPolyline2View<T> ordered(std::vector<const Vec2<T>*>&& vertices) noexcept;

        ConstPolyline2View(const std::vector<const Vec2<T>*>& vertices);
        ConstPolyline2View(std::vector<const Vec2<T>*>&& vertices) noexcept;

        ConstPolyline2View(const ConstPolyline2View<T>& other);
        ConstPolyline2View(ConstPolyline2View<T>&& other) noexcept;

        ConstPolyline2View<T>& operator=(const ConstPolyline2View<T>& other) &;
        ConstPolyline2View<T>& operator=(ConstPolyline2View<T>&& other) & noexcept;

        const Vec2<T>& vertex(int i) const;

        const Vec2<T>* ptrToVertex(int i) const;

        void setPtrToVertex(int i, const Vec2<T>* newPtr) &;

        void addVertex(const Vec2<T>* newVertex) &;
        template <class InputIt>
        void addVertices(InputIt begin, InputIt end) &;

        void sort() &;

        int numberOfVertices() const;

        Polyline2<T> toPolyline() const;
    private:
        std::vector<const Vec2<T>*> m_v;
    };

    template <class T>
    bool operator==(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs);
    template <class T>
    bool operator!=(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs);
    template <class T>
    bool operator<(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator<=(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>=(const ConstPolyline2View<T>& lhs, const ConstPolyline2View<T>& rhs); //compares pointers lexicographically

    using ConstPolyline2ViewD = ConstPolyline2View<double>;
    using ConstPolyline2ViewF = ConstPolyline2View<float>;

    extern template class ConstPolyline2View<double>;
    extern template class ConstPolyline2View<float>;
}
#include "../src/Polyline2.cpp"
