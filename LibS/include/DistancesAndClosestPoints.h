#pragma once

#include "..\Fwd.h"

#include "..\Shapes.h"

namespace ls
{
    //2D shapes
    //Vec2
    template <class T>
    T distance(const Vec2<T>& a, const Circle2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const LineSegment2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Polygon2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Polyline2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Ray2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Vec2<T>& a, const Vec2<T>& b);

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
    T distance(const LineSegment2<T>& a, const Vec2<T>& b);

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
    T distance(const Rectangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    T distance(const Rectangle2<T>& a, const Vec2<T>& b);

    //Circle2
    template <class T>
    T distance(const Circle2<T>& a, const Circle2<T>& b);
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
    T distance(const Circle2<T>& a, const Vec2<T>& b);

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
    Vec2<T> closestPoint(const Rectangle2<T>& shape, const Rectangle2<T>& query);
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
    std::pair<Vec2<T>, Vec2<T>> closestPoints(const Vec2<T>& a, const Rectangle2<T>& b);
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

#include "../src/DistancesAndClosestPoints.cpp"

