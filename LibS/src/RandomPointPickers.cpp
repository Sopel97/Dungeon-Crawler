#include "..\include\RandomPointPickers.h"

namespace ls
{
    template <class T>
    RandomPointPicker<Circle2, T, RandomPointPickerTargetAreaTag>::RandomPointPicker(const Circle2<T>& circle) noexcept :
        m_circle(circle)
    {
    }
    template <class T>
    RandomPointPicker<Circle2, T, RandomPointPickerTargetAreaTag>::RandomPointPicker(Circle2<T>&& circle) noexcept :
        m_circle(std::move(circle))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<Circle2, T, RandomPointPickerTargetAreaTag>::nextPoint(RNG&& rng) const
    {
        static constexpr std::uniform_real_distribution<T> radiusSquaredDistribution(T(0), T(1)); 
        static constexpr std::uniform_real_distribution<T> angleDistribution(T(0), T(2) * PI);

        const T r = std::sqrt(radiusSquaredDistribution(std::forward<RNG>(rng))) * m_circle.radius;
        const Angle2D angle = Angle2D::radians(angleDistribution(std::forward<RNG>(rng)));
        return m_circle.origin + Vec2<T>(r * angle.cos(), r * angle.sin());
    }

    template <class T>
    RandomPointPicker<Circle2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(const Circle2<T>& circle) noexcept :
        m_circle(circle)
    {
    }
    template <class T>
    RandomPointPicker<Circle2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(Circle2<T>&& circle) noexcept :
        m_circle(std::move(circle))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<Circle2, T, RandomPointPickerTargetPerimeterTag>::nextPoint(RNG&& rng) const
    {
        static constexpr std::uniform_real_distribution<T> angleDistribution(T(0), T(2) * PI);

        const Angle2D angle = Angle2D::radians(angleDistribution(std::forward<RNG>(rng)));
        return m_circle.origin + Vec2<T>(angle.cos(), angle.sin());
    }

    template <class T>
    RandomPointPicker<LineSegment2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(const LineSegment2<T>& lineSegment) noexcept :
        m_lineSegment(lineSegment)
    {
    }
    template <class T>
    RandomPointPicker<LineSegment2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(LineSegment2<T>&& lineSegment) noexcept :
        m_lineSegment(std::move(lineSegment))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<LineSegment2, T, RandomPointPickerTargetPerimeterTag>::nextPoint(RNG&& rng) const
    {
        static constexpr std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T t = scalarDistribution(std::forward<RNG>(rng));
        return m_lineSegment.begin + (m_lineSegment.end - m_lineSegment.begin) * t;
    }

    /*
    template <class T>
    RandomPointOnPolygonPicker<T>::RandomPointOnPolygonPicker(Polygon2<T> polygon) :
        m_polygon(std::move(polygon)),
        m_triangulation(m_polygon)
    {
        m_trianglesByArea.reserve(m_triangulation.triangleMesh().size());
        for(auto& triangle : m_triangulation.triangleMesh().elements)
        {
            m_trianglesByArea.push_back(std::make_pair(&triangle, triangle.area()));
        }
        int numberOfTriangles = m_trianglesByArea.size();
        for(int i = 1; i < numberOfTriangles; ++i)
        {
            m_trianglesByArea[i].second += m_trianglesByArea[i - 1].second; //so the area is cumulative
        }
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointOnPolygonPicker<T>::nextPoint(RNG&& rng) const
    {
        T sumOfAreas = m_trianglesByArea.back().second;
        std::uniform_real_distribution<T> areaDistribution(T(0), sumOfAreas);
        T randomArea = areaDistribution(std::forward<RNG>(rng));
        auto chosenTriangleIter = std::upper_bound(m_trianglesByArea.begin(), m_trianglesByArea.end(), randomArea, [] (const T& lhs, const std::pair<const Triangle2<T>*, T>& rhs)->bool {return lhs < rhs.second; });
        if(chosenTriangleIter == m_trianglesByArea.end()) chosenTriangleIter = m_trianglesByArea.begin();
        return RandomPointOnTrianglePicker<T>(*(chosenTriangleIter->first)).nextPoint(std::forward<RNG>(rng));
    }
    */

    template <class T>
    RandomPointPicker<Polyline2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(const Polyline2<T>& polyline) :
        m_polyline(polyline)
    {
    }
    template <class T>
    RandomPointPicker<Polyline2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(Polyline2<T>&& polyline) noexcept :
        m_polyline(std::move(polyline))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<Polyline2, T, RandomPointPickerTargetPerimeterTag>::nextPoint(RNG&& rng) const
    {

    }

    template <class T>
    RandomPointPicker<Rectangle2, T, RandomPointPickerTargetAreaTag>::RandomPointPicker(const Rectangle2<T>& rectangle) noexcept :
        m_rectangle(rectangle)
    {
    }
    template <class T>
    RandomPointPicker<Rectangle2, T, RandomPointPickerTargetAreaTag>::RandomPointPicker(Rectangle2<T>&& rectangle) noexcept :
        m_rectangle(std::move(rectangle))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<Rectangle2, T, RandomPointPickerTargetAreaTag>::nextPoint(RNG&& rng) const
    {
        static constexpr std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T t1 = scalarDistribution(std::forward<RNG>(rng));
        const T t2 = scalarDistribution(std::forward<RNG>(rng));
        Vec2<T> displacement = m_rectangle.max - m_rectangle.min;
        displacement.x *= t1;
        displacement.y *= t2;
        return m_rectangle.min + displacement;
    }

    template <class T>
    RandomPointPicker<Rectangle2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(const Rectangle2<T>& rectangle) noexcept :
        m_rectangle(rectangle)
    {
    }
    template <class T>
    RandomPointPicker<Rectangle2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(Rectangle2<T>&& rectangle) noexcept :
        m_rectangle(std::move(rectangle))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<Rectangle2, T, RandomPointPickerTargetPerimeterTag>::nextPoint(RNG&& rng) const
    {
        static constexpr std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T width = m_rectangle.width();
        const T height = m_rectangle.height();
        std::uniform_real_distribution<T> lengthAlongPerimeterDistribution(T(0), width + height);
        std::bernoulli_distribution doOppositeEdgeDistribution(0.5);

        Vec2<T> point = m_rectangle.min;

        const bool doOppositEdge = doOppositeEdgeDistribution(std::forward<RNG>(rng));
        if(lengthAlongPerimeterDistribution(std::forward<RNG>(rng)) > width) //horizontally
        {
            point.x += scalarDistribution(std::forward<RNG>(rng)) * width;
            if(doOppositEdge) point.y += height;
        }
        else //vertically
        {
            point.y += scalarDistribution(std::forward<RNG>(rng)) * height;
            if(doOppositEdge) point.x += width;
        }

        return point;
    }

    template <class T>
    RandomPointPicker<Triangle2, T, RandomPointPickerTargetAreaTag>::RandomPointPicker(const Triangle2<T>& triangle) noexcept :
        m_triangle(triangle)
    {
    }
    template <class T>
    RandomPointPicker<Triangle2, T, RandomPointPickerTargetAreaTag>::RandomPointPicker(Triangle2<T>&& triangle) noexcept :
        m_triangle(std::move(triangle))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<Triangle2, T, RandomPointPickerTargetAreaTag>::nextPoint(RNG&& rng) const
    {
        static constexpr std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T t1 = scalarDistribution(std::forward<RNG>(rng));
        const T t2 = scalarDistribution(std::forward<RNG>(rng));
        const Vec2<T> point(m_triangle.vertices[0] + (m_triangle.vertices[2] - m_triangle.vertices[0]) * t1 + (m_triangle.vertices[1] - m_triangle.vertices[0]) * t2);
        const Vec2<T> e12 = m_triangle.vertiecs[2] - m_triangle.vertices[1];
        const Vec2<T> e10 = m_triangle.vertices[0] - m_triangle.vertices[1];
        const Vec2<T> e1p = point - m_triangle.vertices[1];
        if(e12.cross(e1p) * e12.cross(e10) < T(0)) //points vertices[0] and point are on the opposite sides of the diagonal of the parallelogram
        {
            const Vec2<T> edgeCenter = m_triangle.vertices[1] + (e12 * T(0.5));
            point = edgeCenter * T(2) - point;
        }
        return point;
    }


    template <class T>
    RandomPointPicker<Triangle2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(const Triangle2<T>& triangle) noexcept :
        m_triangle(triangle)
    {
    }
    template <class T>
    RandomPointPicker<Triangle2, T, RandomPointPickerTargetPerimeterTag>::RandomPointPicker(Triangle2<T>&& triangle) noexcept :
        m_triangle(std::move(triangle))
    {
    }
    template <class T>
    template <class RNG>
    Vec2<T> RandomPointPicker<Triangle2, T, RandomPointPickerTargetPerimeterTag>::nextPoint(RNG&& rng) const
    {
        static constexpr std::uniform_real_distribution<T> scalarDistribution(T(0), T(1));

        const T lengths[3] = {
            m_triangle.vertices[0].distanceTo(m_triangle.vertices[1]),
            m_triangle.vertices[1].distanceTo(m_triangle.vertices[2]),
            m_triangle.vertices[2].distanceTo(m_triangle.vertices[0])
        };

        T lengthSums[3];
        lengthSums[0] = lengths[0];
        lengthSums[1] = lengthSums[0] + length[1];
        lengthSums[2] = lengthSums[1] + length[2];

        const T perimeterLength = lengths[0] + lengths[1] + lengths[2];

        std::uniform_real_distribution<T> lengthAlongPerimeterDistribution(T(0), perimeterLength);
        const T lengthAlongPerimeter = lengthAlongPerimeterDistribution(std::forward<RNG>(rng));
        int edgeIndex = 0;
        if(lengthAlongPerimeter > lengthSums[1]) egdeIndex = 2;
        else if(lengthAlongPerimeter > lengthSums[0]) edgeIndex = 1;

        const Vec2<T> begin = m_triangle.vertices[edgeIndex];
        const Vec2<T> end = m_triangle.vertices[(edgeIndex + 1) % 3];
        const T t = scalarDistribution(std::forward<RNG>(rng));
        return begin + (end - begin) * t;
    }
}