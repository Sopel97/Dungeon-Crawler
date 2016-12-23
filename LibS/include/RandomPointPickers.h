#pragma once

#include "..\Fwd.h"

#include "..\Shapes.h"

#include <random>

namespace ls
{
    struct RandomPointPickerTargetAreaTag;
    struct RandomPointPickerTargetPerimeterTag;

    template <template <class> class ShapeT, class T, class TargetTag = RandomPointPickerTargetAreaTag>
    class RandomPointPicker
    {
        RandomPointPicker() = delete;
    };

    template <class T>
    class RandomPointPicker<Circle2, T, RandomPointPickerTargetAreaTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        explicit RandomPointPicker(const Circle2<T>& circle) noexcept;
        explicit RandomPointPicker(Circle2<T>&& circle) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        Circle2<T> m_circle;
    };

    template <class T>
    class RandomPointPicker<Circle2, T, RandomPointPickerTargetPerimeterTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        explicit RandomPointPicker(const Circle2<T>& circle) noexcept;
        explicit RandomPointPicker(Circle2<T>&& circle) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        Circle2<T> m_circle;
    };

    template <class T>
    class RandomPointPicker<LineSegment2, T, RandomPointPickerTargetPerimeterTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        explicit RandomPointPicker(const LineSegment2<T>& lineSegment) noexcept;
        explicit RandomPointPicker(LineSegment2<T>&& lineSegment) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        LineSegment2<T> m_lineSegment;
    };

    /* TODO: rework the internal structure
    template <class T>
    class RandomPointOnPolygonPicker
    {
    public:
        explicit RandomPointOnPolygonPicker(Polygon2<T> polygon);
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;
    protected:
        Polygon2<T> m_polygon;
        PolygonTriangulation<T> m_triangulation;
        std::vector<std::pair<const Triangle2<T>*, T>> m_trianglesByArea; //<triangle ptr to triangulation result, CUMULATIVE area> - cumulative area so binary searching is possible without sorting
    };
    */

    template <class T>
    class RandomPointPicker<Polyline2, T, RandomPointPickerTargetPerimeterTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    public:
        explicit RandomPointPicker(const Polyline2<T>& polyline);
        explicit RandomPointPicker(Polyline2<T>&& polyline) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        Polyline2<T> m_polyline;
    };

    template <class T>
    class RandomPointPicker<Rectangle2, T, RandomPointPickerTargetAreaTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        explicit RandomPointPicker(const Rectangle2<T>& rectangle) noexcept;
        explicit RandomPointPicker(Rectangle2<T>&& rectangle) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        Rectangle2<T> m_rectangle;
    };

    template <class T>
    class RandomPointPicker<Rectangle2, T, RandomPointPickerTargetPerimeterTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        explicit RandomPointPicker(const Rectangle2<T>& rectangle) noexcept;
        explicit RandomPointPicker(Rectangle2<T>&& rectangle) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        Rectangle2<T> m_rectangle;
    };

    template <class T>
    class RandomPointPicker<Triangle2, T, RandomPointPickerTargetAreaTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        explicit RandomPointPicker(const Triangle2<T>& triangle) noexcept;
        explicit RandomPointPicker(Triangle2<T>&& triangle) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        Triangle2<T> m_triangle;
    };

    template <class T>
    class RandomPointPicker<Triangle2, T, RandomPointPickerTargetPerimeterTag>
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        explicit RandomPointPicker(const Triangle2<T>& triangle) noexcept;
        explicit RandomPointPicker(Triangle2<T>&& triangle) noexcept;
        template <class RNG>
        Vec2<T> nextPoint(RNG&& rng) const;

    protected:
        Triangle2<T> m_triangle;
    };
}
#include "../src/RandomPointPickers.cpp"
