#include "..\include\Raycaster2.h"

namespace ls
{
    template <class T>
    Raycast2<T> Raycaster2::raycast(const Ray2<T>& ray, const Circle2<T>& circle)
    {
        const T& dx = ray.direction.x;
        const T& dy = ray.direction.y;
        const T& px = ray.origin.x;
        const T& py = ray.origin.y;
        const T& cx = circle.origin.x;
        const T& cy = circle.origin.y;
        const T& r = circle.radius;

        T a = dx * dx + dy * dy;
        T b = T(2) * px * dx - T(2) * cx * dx + T(2) * py * dy - T(2) * cy * dy;
        T c = -(r * r - px * px - cx * cx - py * py - cy * cy + T(2) * py * cy + T(2) * px * cx);
        T delta = b * b - T(4) * a * c;
        if(delta < T(0)) Raycast2<T>::empty(ray);

        T sqrtDelta = sqrt(delta);
        T t1 = (-b - sqrtDelta) / (T(2) * a);
        T t2 = (-b + sqrtDelta) / (T(2) * a);

        std::vector<typename Raycast2<T>::Hit> hits;
        if(t1 < t2) std::swap(t1, t2);
        if(t1 >= T(0))
        {
            Vec2<T> collisionPoint = ray.origin + ray.direction * t1;
            Vec2<T> n = ((circle.origin - collisionPoint) * T(-1)).normalized();
            hits.push_back(typename Raycast2<T>::Hit{t1, n, collisionPoint});
        }
        if(t2 >= T(0))
        {
            Vec2<T> collisionPoint = ray.origin + ray.direction * t2;
            Vec2<T> n = ((circle.origin - collisionPoint) * T(-1)).normalized();
            hits.push_back(typename Raycast2<T>::Hit{t2, n, collisionPoint});
        }
        return Raycast2<T>(ray, std::move(hits));
    }
    template <class T>
    Raycast2<T> Raycaster2::raycast(const Ray2<T>& ray, const ShapeMesh2<Circle2<T>>& circleMesh)
    {
        std::vector<typename Raycast2<T>::Hit> hits;
        for(const auto& circle : circleMesh.m_shapes)
        {
            const T& dx = ray.direction.x;
            const T& dy = ray.direction.y;
            const T& px = ray.origin.x;
            const T& py = ray.origin.y;
            const T& cx = circle.origin.x;
            const T& cy = circle.origin.y;
            const T& r = circle.radius;

            T a = dx * dx + dy * dy;
            T b = T(2) * px * dx - T(2) * cx * dx + T(2) * py * dy - T(2) * cy * dy;
            T c = -(r * r - px * px - cx * cx - py * py - cy * cy + T(2) * py * cy + T(2) * px * cx);
            T delta = b * b - T(4) * a * c;
            if(delta < T(0)) continue;

            T sqrtDelta = sqrt(delta);
            T t1 = (-b - sqrtDelta) / (T(2) * a);
            T t2 = (-b + sqrtDelta) / (T(2) * a);

            if(t1 >= T(0))
            {
                Vec2<T> collisionPoint = ray.origin + ray.direction * t1;
                Vec2<T> n = ((circle.origin - collisionPoint) * -1.0).normalized();
                hits.push_back(typename Raycast2<T>::Hit{t1, n, collisionPoint});
            }
            if(t2 >= T(0))
            {
                Vec2<T> collisionPoint = ray.origin + ray.direction * t2;
                Vec2<T> n = ((circle.origin - collisionPoint) * -1.0).normalized();
                hits.push_back(typename Raycast2<T>::Hit{t2, n, collisionPoint});
            }
        }
        Raycast2<T> cast(ray, std::move(hits));
        cast.sortHitPoints();
        return cast;
    }
    template <class T>
    Raycast2<T> Raycaster2::raycast(const Ray2<T>& ray, const LineSegment2<T>& lineSegment)
    {
        const T& rdx = ray.direction.x;
        const T& rdy = ray.direction.y;
        const T& rox = ray.origin.x;
        const T& roy = ray.origin.y;

        const T& sbx = lineSegment.begin.x;
        const T& sby = lineSegment.begin.y;

        Vec2<T> sd = (lineSegment.end - lineSegment.begin);
        const T& sdx = sd.x;
        const T& sdy = sd.y;

        /*
            v1 = ray.origin - lineSegment.begin
            v2 = lineSegment.end - lineSegment.begin
            v3 = ray.direction.normal() //perpendicular
            t1 = (v1 . v3) / (v2 . v3)
        */
        T t1 = (rdx * (sby - roy) + rdy * (rox - sbx)) / (sdx * rdy - sdy * rdx);
        T t0 = (sbx + sdx * t1 - rox) / rdx;
        if(t0 < T(0) || t1 < T(0) || t1 > T(1)) return Raycast2<T>::empty(ray);
        Vec2<T> collisionPoint = ray.origin + ray.direction * t0;
        return Raycast2<T>(ray, std::vector<typename Raycast2<T>::Hit> {t0, (lineSegment.end - lineSegment.begin).normalRight(), collisionPoint});
    }
    template <class T>
    Raycast2<T> Raycaster2::raycast(const Ray2<T>& ray, const Polygon2<T>& polygon)
    {
        std::vector<typename Raycast2<T>::Hit> hits;
        int numberOfEdges = polygon.size();
        for(int i = 0; i < numberOfEdges; ++i)
        {
            LineSegmentD edge{polygon.m_vertices[i], polygon.m_vertices[(i + 1) % numberOfEdges]};

            //LineSegment2 raycast
            const T& rdx = ray.direction.x;
            const T& rdy = ray.direction.y;
            const T& rox = ray.origin.x;
            const T& roy = ray.origin.y;

            const T& sbx = edge.begin.x;
            const T& sby = edge.begin.y;

            Vec2<T> sd = (edge.end - edge.begin);
            const T& sdx = sd.x;
            const T& sdy = sd.y;

            T t1 = (rdx * (sby - roy) + rdy * (rox - sbx)) / (sdx * rdy - sdy * rdx);
            T t0 = (sbx + sdx * t1 - rox) / rdx;
            if(t0 < T(0) || t1 < T(0) || t1 > T(1)) continue;
            Vec2<T> collisionPoint = ray.origin + ray.direction * t0;
            hits.push_back(typename Raycast2<T>::Hit{t0, (edge.end - edge.begin).normalRight(), collisionPoint});
        }
        Raycast2<T> cast(ray, std::move(hits));
        cast.sortHitPointsByDistance();
        return cast;
    }
}