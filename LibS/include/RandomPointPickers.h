#ifndef RANDOMPOINTPICKERS_H
#define RANDOMPOINTPICKERS_H

template <class T>
class RandomPointOnCirclePicker
{
public:
    RandomPointOnCirclePicker(const Circle<T>& circle);
    template <class RNG>
    Vec2<T> nextPoint(RNG& rng) const;
protected:
    const Circle<T>& m_circle;
};

template <class T>
class RandomPointOnLineSegmentPicker
{
public:
    RandomPointOnLineSegmentPicker(const LineSegment<T>& lineSegment);
    template <class RNG>
    Vec2<T> nextPoint(RNG& rng) const;
protected:
    const LineSegment<T>& m_lineSegment;
};

template <class T>
class RandomPointOnPolygonPicker
{
public:
    RandomPointOnPolygonPicker(const Polygon<T>& polygon);
    template <class RNG>
    Vec2<T> nextPoint(RNG& rng) const;
protected:
    const Polygon<T>& m_polygon;
    PolygonTriangulation<T> m_triangulation;
    std::vector<std::pair<const Triangle<T>*, T>> m_trianglesByArea; //<triangle ptr to triangulation result, CUMULATIVE area> - cumulative area so binary searching is possible without sorting
};

template <class T>
class RandomPointOnPolylinePicker
{
public:
    RandomPointOnPolylinePicker(const Polyline<T>& polyline);
    template <class RNG>
    Vec2<T> nextPoint(RNG& rng) const;
protected:
    const Polyline<T>& m_polyline;
};

template <class T>
class RandomPointOnRectanglePicker
{
public:
    RandomPointOnRectanglePicker(const Rectangle<T>& rectangle);
    template <class RNG>
    Vec2<T> nextPoint(RNG& rng) const;
protected:
    const Rectangle<T>& m_rectangle;
};

template <class T>
class RandomPointOnTrianglePicker
{
public:
    RandomPointOnTrianglePicker(const Triangle<T>& triangle);
    template <class RNG>
    Vec2<T> nextPoint(RNG& rng) const;
protected:
    const Triangle<T>& m_triangle;
};

#include "../src/RandomPointPickers.cpp"

#endif // RANDOMPOINTPICKERS_H
