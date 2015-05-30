#ifndef RAYCASTER_H
#define RAYCASTER_H

class Raycaster
{
public:
    Raycaster() = delete;

    template <class T>
    static Raycast<T> raycast(const Ray<T>& ray, const Circle<T>& circle);
    template <class T>
    static Raycast<T> raycast(const Ray<T>& ray, const Mesh2<Circle<T>>& circleMesh);
    template <class T>
    static Raycast<T> raycast(const Ray<T>& ray, const LineSegment<T>& lineSegment);
    template <class T>
    static Raycast<T> raycast(const Ray<T>& ray, const Polygon<T>& polygon);
};

#include "../src/Raycaster.cpp"

#endif // RAYCASTER_H
