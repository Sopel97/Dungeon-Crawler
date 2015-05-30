#ifndef INTERSECTIONS_H_INCLUDED
#define INTERSECTIONS_H_INCLUDED

class Intersections
{
public:

    template <class Shape>
    class LinearSweep //may be better to move it to Geo:: rather than leaving it in Intersections::
    {
    public:
        using VectorType = typename Shape::RespectiveVectorType;
        using ShapeType = Shape;

        LinearSweep(const Shape& shape, const VectorType& velocity);
        //don't know if enabling following is a good idea since initilalShape and finalShape may differ in rotation (which is not handled by this class.
        //LinearSweep(const Shape& initialShape, const Shape& finalShape, double deltaTime);
        const Shape& initialShape() const;
        const VectorType& velocity() const;

        void setVelocity(const VectorType& newVelocity);
        void setInitialShape(const Shape& newInitialShape);

        Shape after(double time) const;

    private:
        const Shape* m_initialShape;
        VectorType m_velocity;
    };

    template <class SweepA, class SweepB>
    class SweepCollision
    {
    public:
        SweepCollision(const SweepA& sweepA, const SweepB& sweepB, double time);
        SweepCollision(SweepA&& sweepA, SweepB&& sweepB, double time);

        double timeTillCollision() const;
        const SweepA& firstShapeSweep() const;
        const SweepB& secondShapeSweep() const;

        typename SweepA::ShapeType firstShapeOnCollisionTime() const;
        typename SweepB::ShapeType secondShapeOnCollisionTime() const;

        bool anyCollision() const;

    private:
        const SweepA m_firstShapeSweep;
        const SweepB m_secondShapeSweep;
        double m_timeTillCollision; //storing all collisions may not be needed. This will the closest one in time
    };

    Intersections() = delete;

    //2D shapes
    //Vec2
    template <class T>
    static bool intersection(const Vec2<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const Vec2<T>& a, const Vec2<T>& b); // temporarly made a margin of some size to make it able to be picked by mouse in InteractiveShape

    //LineSegment
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const LineSegment<T>& b, Vec2<T>& intersectionPoint); //later there will more kinds of intersections and this will be handled there
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const LineSegment<T>& a, const Vec2<T>& b);

    //Rectangle
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const Rectangle<T>& a, const Vec2<T>& b);

    //Circle
    template <class T>
    static bool intersection(const Circle<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const Circle<T>& a, const Vec2<T>& b);

    //Triangle
    template <class T>
    static bool intersection(const Triangle<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Triangle<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Triangle<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Triangle<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const Triangle<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const Triangle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Triangle<T>& a, const Triangle<T>& b); //need to be redone better
    template <class T>
    static bool intersection(const Triangle<T>& a, const Vec2<T>& b);

    //Polygon
    template <class T>
    static bool intersection(const Polygon<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const Polygon<T>& a, const Vec2<T>& b);

    //Polyline
    template <class T>
    static bool intersection(const Polyline<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Polyline<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Polyline<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Polyline<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const Polyline<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const Polyline<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Polyline<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const Polyline<T>& a, const Vec2<T>& b);

    //Ray
    template <class T>
    static bool intersection(const Ray<T>& a, const Circle<T>& b);
    template <class T>
    static bool intersection(const Ray<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool intersection(const Ray<T>& a, const Polygon<T>& b);
    template <class T>
    static bool intersection(const Ray<T>& a, const Polyline<T>& b);
    template <class T>
    static bool intersection(const Ray<T>& a, const Ray<T>& b);
    template <class T>
    static bool intersection(const Ray<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool intersection(const Ray<T>& a, const Triangle<T>& b);
    template <class T>
    static bool intersection(const Ray<T>& a, const Vec2<T>& b);

    //3D shapes
    //Vec3

    //Cylinder
    template <class T>
    static bool intersection(const Cylinder<T>& a, const Cylinder<T>& b);

    //Cuboid
    template <class T>
    static bool intersection(const Cuboid<T>& a, const Cuboid<T>& b);


    //Meshes
    template <class T, class V>
    static bool intersection(const Mesh2<T>& a, const Mesh2<V>& b);
    template <class T, class S>
    static bool intersection(const Mesh2<T>& a, const S& b);
    template <class T, class S>
    static bool intersection(const S& a, const Mesh2<T>& b);

    //Polymorphic
    template <class T>
    static bool intersection(const Shape2<T>& a, const Shape2<T>& b);
    template <class T, class S>
    static bool intersection(const Shape2<T>& a, const S& b);
    template <class T, class S>
    static bool intersection(const S& a, const Shape2<T>& b);


    /*
        Check whether a contains b.
        Shapes with no area always return false
        This operation, unline intersection, is not not symmetric.
        If contains(a,b) is true, then the reverse must be false.
        But if contains(a,b) is false then the reverse can either be true or false
    */


    //WARNING: Some contains are not defined. They will always return false!!!

    //2D shapes
    //Vec2
    template <class T>
    static bool contains(const Vec2<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool contains(const Vec2<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool contains(const Vec2<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool contains(const Vec2<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool contains(const Vec2<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const Vec2<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool contains(const Vec2<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool contains(const Vec2<T>& a, const Vec2<T>& b){return false;}

    //LineSegment
    template <class T>
    static bool contains(const LineSegment<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool contains(const LineSegment<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool contains(const LineSegment<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool contains(const LineSegment<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool contains(const LineSegment<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const LineSegment<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool contains(const LineSegment<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool contains(const LineSegment<T>& a, const Vec2<T>& b){return false;}

    //Rectangle
    template <class T>
    static bool contains(const Rectangle<T>& a, const Circle<T>& b);
    template <class T>
    static bool contains(const Rectangle<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool contains(const Rectangle<T>& a, const Polygon<T>& b);
    template <class T>
    static bool contains(const Rectangle<T>& a, const Polyline<T>& b);
    template <class T>
    static bool contains(const Rectangle<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const Rectangle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool contains(const Rectangle<T>& a, const Triangle<T>& b);
    template <class T>
    static bool contains(const Rectangle<T>& a, const Vec2<T>& b);

    //Circle
    template <class T>
    static bool contains(const Circle<T>& a, const Circle<T>& b);
    template <class T>
    static bool contains(const Circle<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool contains(const Circle<T>& a, const Polygon<T>& b);
    template <class T>
    static bool contains(const Circle<T>& a, const Polyline<T>& b);
    template <class T>
    static bool contains(const Circle<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const Circle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool contains(const Circle<T>& a, const Triangle<T>& b);
    template <class T>
    static bool contains(const Circle<T>& a, const Vec2<T>& b);

    //Triangle
    template <class T>
    static bool contains(const Triangle<T>& a, const Circle<T>& b);
    template <class T>
    static bool contains(const Triangle<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool contains(const Triangle<T>& a, const Polygon<T>& b);
    template <class T>
    static bool contains(const Triangle<T>& a, const Polyline<T>& b);
    template <class T>
    static bool contains(const Triangle<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const Triangle<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool contains(const Triangle<T>& a, const Triangle<T>& b);
    template <class T>
    static bool contains(const Triangle<T>& a, const Vec2<T>& b);

    //Polygon
    template <class T>
    static bool contains(const Polygon<T>& a, const Circle<T>& b);
    template <class T>
    static bool contains(const Polygon<T>& a, const LineSegment<T>& b);
    template <class T>
    static bool contains(const Polygon<T>& a, const Polygon<T>& b);
    template <class T>
    static bool contains(const Polygon<T>& a, const Polyline<T>& b);
    template <class T>
    static bool contains(const Polygon<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const Polygon<T>& a, const Rectangle<T>& b);
    template <class T>
    static bool contains(const Polygon<T>& a, const Triangle<T>& b);
    template <class T>
    static bool contains(const Polygon<T>& a, const Vec2<T>& b);

    //Polyline
    template <class T>
    static bool contains(const Polyline<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool contains(const Polyline<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool contains(const Polyline<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool contains(const Polyline<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool contains(const Polyline<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const Polyline<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool contains(const Polyline<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool contains(const Polyline<T>& a, const Vec2<T>& b){return false;}

    //Ray
    template <class T>
    static bool contains(const Ray<T>& a, const Circle<T>& b){return false;}
    template <class T>
    static bool contains(const Ray<T>& a, const LineSegment<T>& b){return false;}
    template <class T>
    static bool contains(const Ray<T>& a, const Polygon<T>& b){return false;}
    template <class T>
    static bool contains(const Ray<T>& a, const Polyline<T>& b){return false;}
    template <class T>
    static bool contains(const Ray<T>& a, const Ray<T>& b){return false;}
    template <class T>
    static bool contains(const Ray<T>& a, const Rectangle<T>& b){return false;}
    template <class T>
    static bool contains(const Ray<T>& a, const Triangle<T>& b){return false;}
    template <class T>
    static bool contains(const Ray<T>& a, const Vec2<T>& b){return false;}

    //Following ones are more complicated than I thought and they may not be implemented correctly for a long time.
    //Meshes
    template <class T, class V>
    static bool contains(const Mesh2<T>& a, const Mesh2<V>& b);
    template <class T, class S>
    static bool contains(const Mesh2<T>& a, const S& b);
    template <class T, class S>
    static bool contains(const S& a, const Mesh2<T>& b);

    //Polymorphic
    template <class T>
    static bool contains(const Shape2<T>& a, const Shape2<T>& b);
    template <class T, class S>
    static bool contains(const Shape2<T>& a, const S& b);
    template <class T, class S>
    static bool contains(const S& a, const Shape2<T>& b);

    // Inverse
    template <class A, class B>
    static bool isContained(const A& a, const B& b){return contains(a,b);} //one is sufficient because it's just an inverse

};
#include "../src/Intersections.cpp"

#endif // INTERSECTIONS_H_INCLUDED
