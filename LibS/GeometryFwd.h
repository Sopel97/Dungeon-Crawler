#ifndef GEOMETRYFWD_H_INCLUDED
#define GEOMETRYFWD_H_INCLUDED

namespace Geo
{
    /*   UTILITY END  */
    template <class T> class Angle;
    typedef Angle<int> AngleI;
    typedef Angle<float> AngleF;
    typedef Angle<double> AngleD;

    template <class T> class Shape;
    typedef Shape<int> ShapeI;
    typedef Shape<float> ShapeF;
    typedef Shape<double> ShapeD;

    template <class T> class Shape2;
    typedef Shape2<int> Shape2I;
    typedef Shape2<float> Shape2F;
    typedef Shape2<double> Shape2D;

    template <class T> class Shape3;
    typedef Shape3<int> Shape3I;
    typedef Shape3<float> Shape3F;
    typedef Shape3<double> Shape3D;

    template <class T> class Shape4;
    typedef Shape4<int> Shape4I;
    typedef Shape4<float> Shape4F;
    typedef Shape4<double> Shape4D;

    template <class T> class Vec2;
    typedef Vec2<int> Vec2I;
    typedef Vec2<float> Vec2F;
    typedef Vec2<double> Vec2D;

    template <class T> class Vec3;
    typedef Vec3<int> Vec3I;
    typedef Vec3<float> Vec3F;
    typedef Vec3<double> Vec3D;

    template <class T> class Vec4;
    typedef Vec4<int> Vec4I;
    typedef Vec4<float> Vec4F;
    typedef Vec4<double> Vec4D;

    template <class T, size_t C, size_t R> class Matrix;
    template <class T> class Rectangle;
    typedef Rectangle<int> RectangleI;
    typedef Rectangle<float> RectangleF;
    typedef Rectangle<double> RectangleD;

    template <class T> class Circle;
    typedef Circle<int> CircleI;
    typedef Circle<float> CircleF;
    typedef Circle<double> CircleD;

    template <class T> class LineSegment;
    typedef LineSegment<int> LineSegmentI;
    typedef LineSegment<float> LineSegmentF;
    typedef LineSegment<double> LineSegmentD;

    template <class T> class Ray;
    typedef Ray<int> RayI;
    typedef Ray<float> RayF;
    typedef Ray<double> RayD;

    template <class T> class Triangle;
    typedef Triangle<int> TriangleI;
    typedef Triangle<float> TriangleF;
    typedef Triangle<double> TriangleD;

    template <class T> class Polygon;
    typedef Polygon<int> PolygonI;
    typedef Polygon<float> PolygonF;
    typedef Polygon<double> PolygonD;

    template <class T> class Polyline;
    typedef Polyline<int> PolylineI;
    typedef Polyline<float> PolylineF;
    typedef Polyline<double> PolylineD;

    template <class ShapeType> class Mesh2;
    template <class T> class Cuboid;
    typedef Cuboid<int> CuboidI;
    typedef Cuboid<float> CuboidF;
    typedef Cuboid<double> CuboidD;

    template <class T> class Cylinder;
    typedef Cylinder<int> CylinderI;
    typedef Cylinder<float> CylinderF;
    typedef Cylinder<double> CylinderD;


    class Intersections;
    template <class T> class Raycast;
    class Raycaster;

    template <class T> class TreeNode;
    template <class T> class GraphNode;

    template <class T> class Triangulation;
    typedef Triangulation<int> TriangulationI;
    typedef Triangulation<float> TriangulationF;
    typedef Triangulation<double> TriangulationD;

    template <class T> class PolygonTriangulation;
    typedef PolygonTriangulation<int> PolygonTriangulationI;
    typedef PolygonTriangulation<float> PolygonTriangulationF;
    typedef PolygonTriangulation<double> PolygonTriangulationD;

    template <class T> class PointSetDelaunayTriangulation;
    typedef PointSetDelaunayTriangulation<int> PointSetDelaunayTriangulationI;
    typedef PointSetDelaunayTriangulation<float> PointSetDelaunayTriangulationF;
    typedef PointSetDelaunayTriangulation<double> PointSetDelaunayTriangulationD;

    template <class T, class NodeType = GraphNode<T>> class EuclideanGraph;
    template <class T, class NodeType = GraphNode<T>> class UrquhartGraph;
    template <class T> class VoronoiDiagram;
    typedef VoronoiDiagram<int> VoronoiDiagramI;
    typedef VoronoiDiagram<float> VoronoiDiagramF;
    typedef VoronoiDiagram<double> VoronoiDiagramD;

    template <class T> class RandomShapePicker;
    typedef RandomShapePicker<int> RandomShapePickerI;
    typedef RandomShapePicker<float> RandomShapePickerF;
    typedef RandomShapePicker<double> RandomShapePickerD;

    template <class T> class ConvexHull;
    typedef ConvexHull<int> ConvexHullI;
    typedef ConvexHull<float> ConvexHullF;
    typedef ConvexHull<double> ConvexHullD;

    template <class T> class Transformation2;
    typedef Transformation2<int> Transformation2I;
    typedef Transformation2<float> Transformation2F;
    typedef Transformation2<double> Transformation2D;

    template <class T> class AffineTransformation;
    typedef AffineTransformation<int> AffineTransformationI;
    typedef AffineTransformation<float> AffineTransformationF;
    typedef AffineTransformation<double> AffineTransformationD;

    template <class T> class BezierCurve;
    typedef BezierCurve<int> BezierCurveI;
    typedef BezierCurve<float> BezierCurveF;
    typedef BezierCurve<double> BezierCurveD;

    template <class T> class BezierPath;
    typedef BezierPath<int> BezierPathI;
    typedef BezierPath<float> BezierPathF;
    typedef BezierPath<double> BezierPathD;

    template <class T> class Matrix3x3;
    enum class OriginalCellularAutomatonStates;
    template <int I = 0> class ElementaryCellularAutomatonT;
    template <class Rules> class CellularAutomaton;


}

#endif // GEOMETRYFWD_H_INCLUDED
