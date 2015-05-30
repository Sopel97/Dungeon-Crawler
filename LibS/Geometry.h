#ifndef GEOMETRY_H_INCLUDED
#define GEOMETRY_H_INCLUDED

#include <vector>
#include <set>
#include <cmath>
#include <utility>
#include <functional>
#include <algorithm>
#include <array>
#include "Array2.h"
#include "Random.h"

//maybe will be renamed to Shapes and some functions will be extracted to other files
//NOTE: 3d shapes are leftover from early stages of the library and will be completly rewritten later.
namespace Geo
{

//for double dispatch of some shape2 functions

#define SHAPE2_PURE_VIRTUAL_DOUBLE_DISPATCHING_METHODS \
    virtual bool intersects(const Shape2<T>* other) const                = 0;\
\
    virtual bool intersects(const Circle<T>& other) const                = 0;\
    virtual bool intersects(const LineSegment<T>& other) const           = 0;\
    virtual bool intersects(const Polygon<T>& other) const               = 0;\
    virtual bool intersects(const Polyline<T>& other) const              = 0;\
    virtual bool intersects(const Ray<T>& other) const                   = 0;\
    virtual bool intersects(const Rectangle<T>& other) const             = 0;\
    virtual bool intersects(const Triangle<T>& other) const              = 0;\
    virtual bool intersects(const Vec2<T>& other) const                  = 0;\
\
    virtual bool intersects(const Mesh2<Circle<T>>& other) const         = 0;\
    virtual bool intersects(const Mesh2<LineSegment<T>>& other) const    = 0;\
    virtual bool intersects(const Mesh2<Polygon<T>>& other) const        = 0;\
    virtual bool intersects(const Mesh2<Polyline<T>>& other) const       = 0;\
    virtual bool intersects(const Mesh2<Ray<T>>& other) const            = 0;\
    virtual bool intersects(const Mesh2<Rectangle<T>>& other) const      = 0;\
    virtual bool intersects(const Mesh2<Triangle<T>>& other) const       = 0;\
    virtual bool intersects(const Mesh2<Vec2<T>>& other) const           = 0;\
\
    virtual bool contains(const Shape2<T>* other) const                  = 0;\
\
    virtual bool contains(const Circle<T>& other) const                  = 0;\
    virtual bool contains(const LineSegment<T>& other) const             = 0;\
    virtual bool contains(const Polygon<T>& other) const                 = 0;\
    virtual bool contains(const Polyline<T>& other) const                = 0;\
    virtual bool contains(const Ray<T>& other) const                     = 0;\
    virtual bool contains(const Rectangle<T>& other) const               = 0;\
    virtual bool contains(const Triangle<T>& other) const                = 0;\
    virtual bool contains(const Vec2<T>& other) const                    = 0;\
\
    virtual bool contains(const Mesh2<Circle<T>>& other) const           = 0;\
    virtual bool contains(const Mesh2<LineSegment<T>>& other) const      = 0;\
    virtual bool contains(const Mesh2<Polygon<T>>& other) const          = 0;\
    virtual bool contains(const Mesh2<Polyline<T>>& other) const         = 0;\
    virtual bool contains(const Mesh2<Ray<T>>& other) const              = 0;\
    virtual bool contains(const Mesh2<Rectangle<T>>& other) const        = 0;\
    virtual bool contains(const Mesh2<Triangle<T>>& other) const         = 0;\
    virtual bool contains(const Mesh2<Vec2<T>>& other) const             = 0;\
\
    virtual bool isContained(const Shape2<T>* other) const               = 0;\
\
    virtual bool isContained(const Circle<T>& other) const               = 0;\
    virtual bool isContained(const LineSegment<T>& other) const          = 0;\
    virtual bool isContained(const Polygon<T>& other) const              = 0;\
    virtual bool isContained(const Polyline<T>& other) const             = 0;\
    virtual bool isContained(const Ray<T>& other) const                  = 0;\
    virtual bool isContained(const Rectangle<T>& other) const            = 0;\
    virtual bool isContained(const Triangle<T>& other) const             = 0;\
    virtual bool isContained(const Vec2<T>& other) const                 = 0;\
\
    virtual bool isContained(const Mesh2<Circle<T>>& other) const        = 0;\
    virtual bool isContained(const Mesh2<LineSegment<T>>& other) const   = 0;\
    virtual bool isContained(const Mesh2<Polygon<T>>& other) const       = 0;\
    virtual bool isContained(const Mesh2<Polyline<T>>& other) const      = 0;\
    virtual bool isContained(const Mesh2<Ray<T>>& other) const           = 0;\
    virtual bool isContained(const Mesh2<Rectangle<T>>& other) const     = 0;\
    virtual bool isContained(const Mesh2<Triangle<T>>& other) const      = 0;\
    virtual bool isContained(const Mesh2<Vec2<T>>& other) const          = 0;

#define SHAPE2_DOUBLE_DISPATCHING_METHODS \
    virtual bool intersects(const Circle<T>& other) const                {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const LineSegment<T>& other) const           {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Polygon<T>& other) const               {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Polyline<T>& other) const              {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Ray<T>& other) const                   {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Rectangle<T>& other) const             {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Triangle<T>& other) const              {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Vec2<T>& other) const                  {return Intersections::intersection(other, *this);}\
\
    virtual bool intersects(const Mesh2<Circle<T>>& other) const         {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Mesh2<LineSegment<T>>& other) const    {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Mesh2<Polygon<T>>& other) const        {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Mesh2<Polyline<T>>& other) const       {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Mesh2<Ray<T>>& other) const            {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Mesh2<Rectangle<T>>& other) const      {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Mesh2<Triangle<T>>& other) const       {return Intersections::intersection(other, *this);}\
    virtual bool intersects(const Mesh2<Vec2<T>>& other) const           {return Intersections::intersection(other, *this);}\
\
    virtual bool contains(const Circle<T>& other) const                  {return Intersections::contains(*this, other);}\
    virtual bool contains(const LineSegment<T>& other) const             {return Intersections::contains(*this, other);}\
    virtual bool contains(const Polygon<T>& other) const                 {return Intersections::contains(*this, other);}\
    virtual bool contains(const Polyline<T>& other) const                {return Intersections::contains(*this, other);}\
    virtual bool contains(const Ray<T>& other) const                     {return Intersections::contains(*this, other);}\
    virtual bool contains(const Rectangle<T>& other) const               {return Intersections::contains(*this, other);}\
    virtual bool contains(const Triangle<T>& other) const                {return Intersections::contains(*this, other);}\
    virtual bool contains(const Vec2<T>& other) const                    {return Intersections::contains(*this, other);}\
\
    virtual bool contains(const Mesh2<Circle<T>>& other) const           {return Intersections::contains(*this, other);}\
    virtual bool contains(const Mesh2<LineSegment<T>>& other) const      {return Intersections::contains(*this, other);}\
    virtual bool contains(const Mesh2<Polygon<T>>& other) const          {return Intersections::contains(*this, other);}\
    virtual bool contains(const Mesh2<Polyline<T>>& other) const         {return Intersections::contains(*this, other);}\
    virtual bool contains(const Mesh2<Ray<T>>& other) const              {return Intersections::contains(*this, other);}\
    virtual bool contains(const Mesh2<Rectangle<T>>& other) const        {return Intersections::contains(*this, other);}\
    virtual bool contains(const Mesh2<Triangle<T>>& other) const         {return Intersections::contains(*this, other);}\
    virtual bool contains(const Mesh2<Vec2<T>>& other) const             {return Intersections::contains(*this, other);}\
\
    virtual bool isContained(const Circle<T>& other) const               {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const LineSegment<T>& other) const          {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Polygon<T>& other) const              {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Polyline<T>& other) const             {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Ray<T>& other) const                  {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Rectangle<T>& other) const            {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Triangle<T>& other) const             {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Vec2<T>& other) const                 {return Intersections::isContained(*this, other);}\
\
    virtual bool isContained(const Mesh2<Circle<T>>& other) const        {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Mesh2<LineSegment<T>>& other) const   {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Mesh2<Polygon<T>>& other) const       {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Mesh2<Polyline<T>>& other) const      {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Mesh2<Ray<T>>& other) const           {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Mesh2<Rectangle<T>>& other) const     {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Mesh2<Triangle<T>>& other) const      {return Intersections::isContained(*this, other);}\
    virtual bool isContained(const Mesh2<Vec2<T>>& other) const          {return Intersections::isContained(*this, other);}

    namespace ___internal
    {
        template <class Type, Type...>
        struct is_one_of;

        template <class Type, Type F>
        struct is_one_of<Type, F>
        {
            static constexpr bool value = false;
        };

        template <class Type, Type F, Type S, Type... T>
        struct is_one_of<Type, F, S, T...>
        {
            static constexpr bool value = F == S
                                          || is_one_of<Type, F, T...>::value;
        };

        template <class Type, Type...>
        struct are_unique;

        template <class Type>
        struct are_unique<Type>
        {
            static constexpr bool value = true;
        };

        template<class Type, Type F, Type... T>
        struct are_unique<Type, F, T...>
        {
            static constexpr bool value = are_unique<Type, T...>::value
                                          && !is_one_of<Type, F, T...>::value;
        };


        template <typename T, template <typename...> class Template>
        struct is_specialization_of
        {
            static constexpr bool value = false;
        };

        template <typename... Args, template <typename...> class Template>
        struct is_specialization_of<Template<Args...>, Template>
        {
            static constexpr bool value = true;
        };
    }
    /*   UTILITY   */
    constexpr double PI = 3.14159265359;
    constexpr double EPSILON = 0.000000001;

    inline double radiansToDegrees(const double radians)
    {
        return radians * (180.0 / PI);
    }
    inline double degreesToRadians(const double degrees)
    {
        return degrees * (PI / 180.0);
    }
    inline double clamp(int a, int mi, int ma)
    {
        return std::max(std::min(a, ma), mi);
    }
    /*   UTILITY END  */
    template <class T> class Angle;
    template <class T> class Shape;
    template <class T> class Shape2;
    template <class T> class Shape3;
    template <class T> class Shape4;
    template <class T> class Vec2;
    template <class T> class Vec3;
    template <class T> class Vec4;
    template <class T, size_t C, size_t R> class Matrix;
    template <class T> class Rectangle;
    template <class T> class Circle;
    template <class T> class LineSegment;
    template <class T> class Ray;
    template <class T> class Triangle;
    template <class T> class Polygon;
    template <class T> class Polyline;
    template <class ShapeType> class Mesh2;
    template <class T> class Cuboid;
    template <class T> class Cylinder;

    class Intersections;
    template <class T> class Raycast;
    class Raycaster;

    template <class T> class TreeNode;
    template <class T> class GraphNode;

    template <class T> class Triangulation;
    template <class T> class PolygonTriangulation;
    template <class T> class PointSetDelaunayTriangulation;
    template <class T, class NodeType = GraphNode<T>> class EuclideanGraph;
    template <class T, class NodeType = GraphNode<T>> class UrquhartGraph;
    template <class T> class VoronoiDiagram;
    template <class T> class RandomShapePicker;
    template <class T> class ConvexHull;
    template <class T> class Transformation2;
    template <class T> class AffineTransformation;
    template <class T> class BezierCurve;
    template <class T> class BezierPath;
    template <class T> class Matrix3x3;
    enum class OriginalCellularAutomatonStates;
    template <int I = 0> class ElementaryCellularAutomatonT;
    template <class Rules> class CellularAutomaton;
    template <class T, class NodeType = TreeNode<T>> class RapidlyExploringRandomTree;

    /*         HEADERS         */

#include "include/Intersections.h"

#include "include/Angle.h"
#include "include/Shape.h"
#include "include/Shape2.h"
#include "include/Shape3.h"
#include "include/Shape4.h"
#include "include/Vec2.h"
#include "include/Vec3.h"
#include "include/Vec4.h"
#include "include/Matrix.h"
#include "include/Rectangle.h"
#include "include/Circle.h"
#include "include/LineSegment.h"
#include "include/Ray.h"
#include "include/Triangle.h"
#include "include/Polygon.h"
#include "include/Polyline.h"
#include "include/Mesh2.h"
#include "include/Cuboid.h"
#include "include/Cylinder.h"

#include "include/Raycast.h"
#include "include/Raycaster.h"

#include "include/Node.h"

#include "include/Triangulation.h"
#include "include/PolygonTriangulation.h"
#include "include/PointSetDelaunayTriangulation.h"
#include "include/EuclideanGraph.h"
#include "include/VoronoiDiagram.h"
#include "include/UrquhartGraph.h"
#include "include/RandomShapePicker.h"
#include "include/ConvexHull.h"
#include "include/Transformation2.h"
#include "include/AffineTransformation.h"
#include "include/BezierCurve.h"
#include "include/BezierPath.h"
#include "include/Matrix3x3.h"
#include "include/ElementaryCellularAutomaton.h"
#include "include/CellularAutomaton.h"
#include "include/RapidlyExploringRandomTree.h"

#undef SHAPE2_DOUBLE_DISPATCHING_METHODS
}
#endif // GEOMETRY_H_INCLUDED
