#pragma once

#include "..\Fwd.h"

#include "..\Shapes.h"

namespace ls
{
    class Raycaster2
    {
    public:
        Raycaster2() = delete;

        template <class T>
        static Raycast2<T> raycast(const Ray2<T>& ray, const Circle2<T>& circle);
        template <class T>
        static Raycast2<T> raycast(const Ray2<T>& ray, const ShapeMesh2<Circle2<T>>& circleMesh);
        template <class T>
        static Raycast2<T> raycast(const Ray2<T>& ray, const LineSegment2<T>& lineSegment);
        template <class T>
        static Raycast2<T> raycast(const Ray2<T>& ray, const Polygon2<T>& polygon);
    };
}

#include "../src/Raycaster2.cpp"
