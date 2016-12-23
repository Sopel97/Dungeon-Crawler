#pragma once

#include "Fwd.h"

#include "Vec2.h"
#include "Triangle2.h"
#include "Circle2.h"
#include "CollisionDetection.h"

#include <vector>
#include <initializer_list>
#include <utility>
#include <algorithm>

namespace ls
{
    template <class T>
    class DelaunayTriangulation2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        using ValueType = T;

        explicit DelaunayTriangulation2(std::initializer_list<Vec2<T>> list);
        explicit DelaunayTriangulation2(const std::vector<Vec2<T>>& vertices);
        explicit DelaunayTriangulation2(std::vector<Vec2<T>>&& vertices);
        template <class InputIt>
        DelaunayTriangulation2(InputIt begin, InputIt end);

        ~DelaunayTriangulation2();

        const Vec2<T>& vertex(int i) const;
        const std::vector<Vec2<T>>& vertices() const &;
        std::vector<Vec2<T>> vertices() &&;

        int numberOfVertices() const;

    protected:

        struct Triangle
        {
            Index3 indices;
            Circle2<T> circumcircle;
        };

        std::vector<Vec2<T>> m_vertices;
        std::vector<Triangle> m_completedTriangles;

        void calculate();
        void sortVertices();
        void insertExistingVertex(int i);
        Triangle2<T> superTriangle() const;
    };

    using DelaunayTriangulation2D = DelaunayTriangulation2<double>;
    using DelaunayTriangulation2F = DelaunayTriangulation2<float>;

    extern template class DelaunayTriangulation2<double>;
    extern template class DelaunayTriangulation2<float>;
}

#include "..\src\DelaunayTriangulation2.cpp"

