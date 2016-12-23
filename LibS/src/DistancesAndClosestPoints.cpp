#include "..\include\DistancesAndClosestPoints.h"

#include "..\Util.h"

namespace ls
{
    //2D shapes
    //Vec2
    template <class T>
    T distance(const Vec2<T>& a, const Circle2<T>& b)
    {
        return std::min(T(0), distance(a, b.origin) - b.radius);
    }
    template <class T>
    T distance(const Vec2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Rectangle2<T>& b)
    {
        return a.distance(closestPoint(b, a));
    }
    template <class T>
    T distance(const Vec2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Vec2<T>& b)
    {
        return a.distance(b);
    }

    //LineSegment2
    template <class T>
    T distance(const LineSegment2<T>& a, const Circle2<T>& b);
    template <class T>
    T distance(const LineSegment2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const LineSegment2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const LineSegment2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const LineSegment2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const LineSegment2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const LineSegment2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const LineSegment2<T>& a, const Vec2<T>& b)
    {
        return distance(b, a);
    }

    //Rectangle2
    template <class T>
    T distance(const Rectangle2<T>& a, const Circle2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const Rectangle2<T>& b)
    {
        Vec2<T> centerA = a.centerOfMass();
        Vec2<T> centerB = b.centerOfMass();
        Vec2<T> displacement = centerA - centerB;
        displacement.x = std::abs(displacement.x);
        displacement.y = std::abs(displacement.y);
        Vec2<T> axisDistances = displacement - Vec2<T>(a.width(), a.height) * T(0.5) - Vec2<T>(b.width(), b.height) * T(0.5);
        axisDistances.x = std::min(T(0), axisDistances.x);
        axisDistances.y = std::min(T(0), axisDistances.y);
        return axisDistances.magnitude();
    }
    template <class T>
    T distance(const Rectangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const Vec2<T>& b)
    {
        return distance(b, a);
    }

    //Circle2
    template <class T>
    T distance(const Circle2<T>& a, const Circle2<T>& b)
    {
        return std::min(T(0), distance(a.origin, b.origin) - a.radius - b.radius);
    }
    template <class T>
    T distance(const Circle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Circle2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Circle2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Circle2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Circle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const Circle2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Circle2<T>& a, const Vec2<T>& b)
    {
        return distance(b, a);
    }

    //Triangle2
    template <class T>
    T distance(const Triangle2<T>& a, const Circle2<T>& b);
    template <class T>
    T distance(const Triangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Triangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Triangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Triangle2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Triangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const Triangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Triangle2<T>& a, const Vec2<T>& b);

    //Polygon2
    template <class T>
    T distance(const Polygon2<T>& a, const Circle2<T>& b);
    template <class T>
    T distance(const Polygon2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Polygon2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Polygon2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Polygon2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Polygon2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const Polygon2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Polygon2<T>& a, const Vec2<T>& b);

    //Polyline2
    template <class T>
    T distance(const Polyline2<T>& a, const Circle2<T>& b);
    template <class T>
    T distance(const Polyline2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Polyline2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Polyline2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Polyline2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Polyline2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const Polyline2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Polyline2<T>& a, const Vec2<T>& b);

    //Ray2
    template <class T>
    T distance(const Ray2<T>& a, const Circle2<T>& b);
    template <class T>
    T distance(const Ray2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Ray2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Ray2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Ray2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Ray2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const Ray2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Ray2<T>& a, const Vec2<T>& b);

    //closest points


    template <class T>
    Vec2<T> closestPoint(const Circle2<T>& shape, const Vec2<T>& query);
    template <class T>
    Vec2<T> closestPoint(const LineSegment2<T>& shape, const Vec2<T>& query);
    template <class T>
    Vec2<T> closestPoint(const Polygon2<T>& shape, const Vec2<T>& query);
    template <class T>
    Vec2<T> closestPoint(const Polyline2<T>& shape, const Vec2<T>& query);
    template <class T>
    Vec2<T> closestPoint(const Ray2<T>& shape, const Vec2<T>& query);
    template <class T>
    Vec2<T> closestPoint(const Rectangle2<T>& shape, const Rectangle2<T>& query)
    {
        return Vec2<T>(Util::clamp(query.x, shape.min.x, shape.max.x), Util::clamp(query.y, shape.min.y, shape.max.y));
    }
    template <class T>
    Vec2<T> closestPoint(const Triangle2<T>& shape, const Vec2<T>& query);
    template <class T>
    Vec2<T> closestPoint(const Vec2<T>& shape, const Vec2<T>& query);

    //2D shapes
    //Vec2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Rectangle2<T>& b)
    {
        return std::make_pair(a, closetPoint(b, a));
    }
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Vec2<T>& b);

    //LineSegment2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const Rectangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const LineSegment2<T>& a, const Vec2<T>& b);

    //Rectangle2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Rectangle2<T>& a, const Vec2<T>& b);

    //Circle2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Circle2<T>& a, const Vec2<T>& b);

    //Triangle2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Triangle2<T>& a, const Vec2<T>& b);

    //Polygon2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const Rectangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polygon2<T>& a, const Vec2<T>& b);

    //Polyline2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const Rectangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Polyline2<T>& a, const Vec2<T>& b);

    //Ray2
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const Circle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const LineSegment2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const Polygon2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const Polyline2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const Ray2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const Rectangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const Triangle2<T>& b);
    template <class T>
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Ray2<T>& a, const Vec2<T>& b);
}
