#pragma once

#include "../optional.h"

#include "..\Fwd.h"

#include "..\Shapes.h"

namespace ls
{
    template <class T>
    class ContinuousCollision2
    {
        static_assert(std::is_floating_point<T>::value, "T must be a floating-point type");
    private:
        Vec2<T> m_point;
        Vec2<T> m_normal;
        T m_timeToImpact;

    public:
        ContinuousCollision2(const Vec2<T>& point, const Vec2<T>& normal, const T& timeToImpact) :
            m_point(point),
            m_normal(normal),
            m_timeToImpact(timeToImpact)
        {

        }

        const Vec2<T>& normal() const
        {
            return m_normal;
        }
        const T& timeToImpact() const
        {
            return m_timeToImpact;
        }
    };

    using ContinuousCollision2D = ContinuousCollision2<double>;
    using ContinuousCollision2F = ContinuousCollision2<float>;

    template <class ShapeT, class VectorT = typename ShapeT::VectorType>
    class Accelerating
    {
    private:
        const ShapeT* m_shape;
        VectorT m_acceleration;
        VectorT m_velocity;
    public:
        Accelerating(const ShapeT& shape, const VectorT& acceleration, const VectorT& velocity) :
            m_shape(&shape),
            m_acceleration(acceleration),
            m_velocity(velocity)
        {

        }

        const ShapeT& shape() const
        {
            return *m_shape;
        }
        const VectorT& acceleration() const
        {
            return m_acceleration;
        }
        const VectorT& velocity() const
        {
            return m_velocity;
        }
    };

    template <class ShapeT, class VectorT = typename ShapeT::VectorType>
    class Moving
    {
    private:
        const ShapeT* m_shape;
        VectorT m_velocity;
    public:

        Moving(const ShapeT& shape, const VectorT& velocity) :
            m_shape(&shape),
            m_velocity(velocity)
        {

        }

        const ShapeT& shape() const
        {
            return *m_shape;
        }
        const VectorT& velocity() const
        {
            return m_velocity;
        }
    };

    template <class ShapeT, class VectorT = typename ShapeT::VectorType>
    Accelerating<ShapeT> accelerating(const ShapeT& shape, const VectorT& acceleration = VectorT::zero(), const VectorT& velocity = VectorT::zero())
    {
        return Accelerating<ShapeT>(shape, acceleration, velocity);
    }
    template <class ShapeT, class VectorT = typename ShapeT::VectorType>
    Moving<ShapeT> moving(const ShapeT& shape, const VectorT& velocity = VectorT::zero())
    {
        return Moving<ShapeT>(shape, velocity);
    }

    template <class T>
    struct PointNormalPair2
    {
        Vec2<T> normal;
        Vec2<T> point;
    };


    //2D shapes
    //Vec2
    template <class T>
    bool intersect(const Vec2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const Vec2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const Vec2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const Vec2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const Vec2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const Vec2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const Vec2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool intersect(const Vec2<T>& a, const Vec2<T>& b);

    //LineSegment2
    template <class T>
    bool intersect(const LineSegment2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const LineSegment2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const LineSegment2<T>& a, const LineSegment2<T>& b, Vec2<T>& intersectionPoint); //later there will more kinds of intersections and this will be handled there
    template <class T>
    bool intersect(const LineSegment2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const LineSegment2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const LineSegment2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const LineSegment2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const LineSegment2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool intersect(const LineSegment2<T>& a, const Vec2<T>& b);

    //Rectangle2
    template <class T>
    bool intersect(const Rectangle2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const Rectangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const Rectangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const Rectangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const Rectangle2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const Rectangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const Rectangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool intersect(const Rectangle2<T>& a, const Vec2<T>& b);

    //Circle2
    template <class T>
    bool intersect(const Circle2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const Circle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const Circle2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const Circle2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const Circle2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const Circle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const Circle2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool intersect(const Circle2<T>& a, const Vec2<T>& b);

    //Triangle2
    template <class T>
    bool intersect(const Triangle2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const Triangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const Triangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const Triangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const Triangle2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const Triangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const Triangle2<T>& a, const Triangle2<T>& b); //need to be redone better
    template <class T>
    bool intersect(const Triangle2<T>& a, const Vec2<T>& b);

    //Polygon2
    template <class T>
    bool intersect(const Polygon2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const Polygon2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const Polygon2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const Polygon2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const Polygon2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const Polygon2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const Polygon2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool intersect(const Polygon2<T>& a, const Vec2<T>& b);

    //Polyline2
    template <class T>
    bool intersect(const Polyline2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const Polyline2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const Polyline2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const Polyline2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const Polyline2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const Polyline2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const Polyline2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool intersect(const Polyline2<T>& a, const Vec2<T>& b);

    //Ray2
    template <class T>
    bool intersect(const Ray2<T>& a, const Circle2<T>& b);
    template <class T>
    bool intersect(const Ray2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool intersect(const Ray2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool intersect(const Ray2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool intersect(const Ray2<T>& a, const Ray2<T>& b);
    template <class T>
    bool intersect(const Ray2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool intersect(const Ray2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool intersect(const Ray2<T>& a, const Vec2<T>& b);

    /*
        Check whether a contains b.
        Shapes with no area always return false
        This operation, unline intersect, is not symmetric.
        If contains(a,b) is true, then the reverse is true only if a == b.
    */


    //WARNING: Some contains are not defined. They will always return false!!!

    //2D shapes
    //Vec2
    template <class T>
    bool contains(const Vec2<T>& a, const Circle2<T>& b) { return false; }
    template <class T>
    bool contains(const Vec2<T>& a, const LineSegment2<T>& b) { return false; }
    template <class T>
    bool contains(const Vec2<T>& a, const Polygon2<T>& b) { return false; }
    template <class T>
    bool contains(const Vec2<T>& a, const Polyline2<T>& b) { return false; }
    template <class T>
    bool contains(const Vec2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const Vec2<T>& a, const Rectangle2<T>& b) { return false; }
    template <class T>
    bool contains(const Vec2<T>& a, const Triangle2<T>& b) { return false; }
    template <class T>
    bool contains(const Vec2<T>& a, const Vec2<T>& b) { return false; }

    //LineSegment2
    template <class T>
    bool contains(const LineSegment2<T>& a, const Circle2<T>& b) { return false; }
    template <class T>
    bool contains(const LineSegment2<T>& a, const LineSegment2<T>& b) { return false; }
    template <class T>
    bool contains(const LineSegment2<T>& a, const Polygon2<T>& b) { return false; }
    template <class T>
    bool contains(const LineSegment2<T>& a, const Polyline2<T>& b) { return false; }
    template <class T>
    bool contains(const LineSegment2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const LineSegment2<T>& a, const Rectangle2<T>& b) { return false; }
    template <class T>
    bool contains(const LineSegment2<T>& a, const Triangle2<T>& b) { return false; }
    template <class T>
    bool contains(const LineSegment2<T>& a, const Vec2<T>& b) { return false; }

    //Rectangle2
    template <class T>
    bool contains(const Rectangle2<T>& a, const Circle2<T>& b);
    template <class T>
    bool contains(const Rectangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool contains(const Rectangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool contains(const Rectangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool contains(const Rectangle2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const Rectangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool contains(const Rectangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool contains(const Rectangle2<T>& a, const Vec2<T>& b);

    //Circle2
    template <class T>
    bool contains(const Circle2<T>& a, const Circle2<T>& b);
    template <class T>
    bool contains(const Circle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool contains(const Circle2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool contains(const Circle2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool contains(const Circle2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const Circle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool contains(const Circle2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool contains(const Circle2<T>& a, const Vec2<T>& b);

    //Triangle2
    template <class T>
    bool contains(const Triangle2<T>& a, const Circle2<T>& b);
    template <class T>
    bool contains(const Triangle2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool contains(const Triangle2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool contains(const Triangle2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool contains(const Triangle2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const Triangle2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool contains(const Triangle2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool contains(const Triangle2<T>& a, const Vec2<T>& b);

    //Polygon2
    template <class T>
    bool contains(const Polygon2<T>& a, const Circle2<T>& b);
    template <class T>
    bool contains(const Polygon2<T>& a, const LineSegment2<T>& b);
    template <class T>
    bool contains(const Polygon2<T>& a, const Polygon2<T>& b);
    template <class T>
    bool contains(const Polygon2<T>& a, const Polyline2<T>& b);
    template <class T>
    bool contains(const Polygon2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const Polygon2<T>& a, const Rectangle2<T>& b);
    template <class T>
    bool contains(const Polygon2<T>& a, const Triangle2<T>& b);
    template <class T>
    bool contains(const Polygon2<T>& a, const Vec2<T>& b);

    //Polyline2
    template <class T>
    bool contains(const Polyline2<T>& a, const Circle2<T>& b) { return false; }
    template <class T>
    bool contains(const Polyline2<T>& a, const LineSegment2<T>& b) { return false; }
    template <class T>
    bool contains(const Polyline2<T>& a, const Polygon2<T>& b) { return false; }
    template <class T>
    bool contains(const Polyline2<T>& a, const Polyline2<T>& b) { return false; }
    template <class T>
    bool contains(const Polyline2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const Polyline2<T>& a, const Rectangle2<T>& b) { return false; }
    template <class T>
    bool contains(const Polyline2<T>& a, const Triangle2<T>& b) { return false; }
    template <class T>
    bool contains(const Polyline2<T>& a, const Vec2<T>& b) { return false; }

    //Ray2
    template <class T>
    bool contains(const Ray2<T>& a, const Circle2<T>& b) { return false; }
    template <class T>
    bool contains(const Ray2<T>& a, const LineSegment2<T>& b) { return false; }
    template <class T>
    bool contains(const Ray2<T>& a, const Polygon2<T>& b) { return false; }
    template <class T>
    bool contains(const Ray2<T>& a, const Polyline2<T>& b) { return false; }
    template <class T>
    bool contains(const Ray2<T>& a, const Ray2<T>& b) { return false; }
    template <class T>
    bool contains(const Ray2<T>& a, const Rectangle2<T>& b) { return false; }
    template <class T>
    bool contains(const Ray2<T>& a, const Triangle2<T>& b) { return false; }
    template <class T>
    bool contains(const Ray2<T>& a, const Vec2<T>& b) { return false; }
    
    // Inverse
    template <class A, class B>
    bool isContained(const A& a, const B& b) { return contains(a, b); } //one is sufficient because it's just an inverse

    /*
    
        Continuous collision detection
        Time to impact computing
        Time to impact estimation
        Normal computing

    */

    template <class T>
    PointNormalPair2<T> collisionPointAndNormal(const Circle2<T>& circle, const Vec2<T>& point);
    template <class T>
    PointNormalPair2<T> collisionPointAndNormal(const Circle2<T>& c1, const Circle2<T>& c2);
    template <class T>
    PointNormalPair2<T> collisionPointAndNormal(const Circle2<T>& circle, const Rectangle2<T>& rectangle);


    template <class T>
    std::experimental::optional<T> timeToImpact(const Moving<Circle2<T>>& movingCircle1, const Moving<Circle2<T>>& movingCircle2);

    template <class T>
    std::experimental::optional<T> timeToImpact(const Moving<Circle2<T>>& movingCircle, const Moving<Vec2<T>>& movingPoint);

    template <class T>
    std::experimental::optional<T> timeToImpact(const Moving<Circle2<T>>& movingCircle, const Moving<Rectangle2<T>>& movingRectangle);

    template <class S1, class S2, class T = typename S1::ValueType>
    std::experimental::optional<ContinuousCollision2<T>> continuousCollision(const Moving<S1>& movingShape1, const Moving<S2>& movingShape2);

    template <class T>
    T estimatedMinTimeToImpact(const Moving<Circle2<T>>& movingCircle1, const Moving<Circle2<T>>& movingCircle2); //timeOfImpact is always none or greater than estimated

}
#include "../src/CollisionDetection.cpp"
