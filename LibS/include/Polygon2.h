#pragma once

#include "..\Fwd.h"

#include "Shape2.h"
#include "Vec2.h"
#include "Polyline2.h"
#include "Rectangle2.h"
#include "Triangle2.h"

#include <vector>
#include <initializer_list>
#include <utility>
#include <algorithm>

namespace ls
{
    template <class T>
    class Polygon2 : public Shape2<T> //Supports convex and concave polygons. Does not support self intersecting (complex) polygons (some funtions may work, some may not).
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        Polygon2() = default;
        explicit Polygon2(std::initializer_list<Vec2<T>> _vertices);
        explicit Polygon2(const std::vector<Vec2<T>>& _vertices);
        explicit Polygon2(std::vector<Vec2<T>>&& _vertices) noexcept(std::is_nothrow_move_constructible<std::vector<Vec2<T>>>::value);
        template <class InputIt>
        Polygon2(InputIt begin, InputIt end);

        Polygon2(const Polygon2<T>& other) = default;
        Polygon2(Polygon2<T>&& other) = default;

        Polygon2<T>& operator=(const Polygon2<T>& rhs) & = default;
        Polygon2<T>& operator=(Polygon2<T> && rhs) & = default;

        //static Polygon2<T> randomInscribedInCircle(const Circle2<T>& circle, int edges); TODO: this
        static Polygon2<T> regular(const Vec2<T>& center, int numberOfEdges, const T& radius);
        static Polygon2<T> fromRectangle(const Rectangle2<T>& rectangle);
        static Polygon2<T> fromTriangle(const Triangle2<T>& triangle);

        Polygon2<T> translated(const Vec2<T>& displacement) const;
        Polygon2<T>& translate(const Vec2<T>& displacement) &;

        template <class T2>
        explicit operator Polygon2<T2>() const;

        template <class... Args>
        void emplaceVertex(Args&&... coords);
        void addVertex(const Vec2<T>& newVertex) &;
        template <class InputIt>
        void addVertices(InputIt begin, InputIt end) &;
        void addVertices(std::initializer_list<Vec2<T>> newVertices) &;

        const std::vector<Vec2<T>>& vertices() const &;
        std::vector<Vec2<T>> vertices() &&;
        const Vec2<T>& vertex(int i) const;
        Vec2<T>& vertex(int i);

        Vec2<T> centerOfMass() const;
        bool isConvex() const;
        T signedArea() const;
        T area() const;

        int numberOfVertices() const;

    protected:
        std::vector<Vec2<T>> m_vertices;
    };

    using Polygon2D = Polygon2<double>;
    using Polygon2F = Polygon2<float>;

    extern template class Polygon2<double>;
    extern template class Polygon2<float>;


    template <class T>
    class ConstPolygon2View //underlying points are immutable
    {
    public:
        using ValueType = T;

        static ConstPolygon2View<T> ordered(const std::vector<const Vec2<T>*>& vertices);
        static ConstPolygon2View<T> ordered(std::vector<const Vec2<T>*>&& vertices) noexcept(std::is_nothrow_constructible<ConstPolygon2View<T>, std::vector<const Vec2<T>*>&&>::value);

        explicit ConstPolygon2View(const std::vector<const Vec2<T>*>& vertices);
        explicit ConstPolygon2View(std::vector<const Vec2<T>*>&& vertices) noexcept(std::is_nothrow_move_constructible<std::vector<const Vec2<T>*>>::value);

        ConstPolygon2View(const ConstPolygon2View<T>& other) = default;
        ConstPolygon2View(ConstPolygon2View<T>&& other) = default;

        ConstPolygon2View<T>& operator=(const ConstPolygon2View<T>& other) & = default;
        ConstPolygon2View<T>& operator=(ConstPolygon2View<T>&& other) & = default;

        const Vec2<T>& vertex(int i) const;
        const std::vector<const Vec2<T>*>& vertices() const &;
        std::vector<const Vec2<T>*> vertices() &&;

        const Vec2<T>* ptrToVertex(int i) const;

        void setPtrToVertex(int i, const Vec2<T>* newPtr) &;

        void addVertex(const Vec2<T>* newVertex) &;
        template <class InputIt>
        void addVertices(InputIt begin, InputIt end) &;

        void sort() &;

        int numberOfVertices() const;

        Polygon2<T> toPolygon() const;
    private:
        std::vector<const Vec2<T>*> m_v;
    };

    template <class T>
    bool operator==(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs);
    template <class T>
    bool operator!=(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs);
    template <class T>
    bool operator<(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator<=(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs); //compares pointers lexicographically
    template <class T>
    bool operator>=(const ConstPolygon2View<T>& lhs, const ConstPolygon2View<T>& rhs); //compares pointers lexicographically

    using ConstPolygon2ViewD = ConstPolygon2View<double>;
    using ConstPolygon2ViewF = ConstPolygon2View<float>;

    extern template class ConstPolygon2View<double>;
    extern template class ConstPolygon2View<float>;

}
#include "../src/Polygon2.cpp"
