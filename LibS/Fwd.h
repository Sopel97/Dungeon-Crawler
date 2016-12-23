#pragma once

namespace ls
{
    template <class T> class BezierCurve2;
    template <class T> class BezierPath2;

    template <class T, int N> class BinomialCoefficients;

    template <int I = 0> class ElementaryCellularAutomatonT;
    template <class Rule> class CellularAutomaton;

    template <class T> class DelaunayTriangulation2;

    template <class T, int Dim> class EuclideanGraphNode;
    template <class T> class WeightedGraphEdge;

    template <int Dim>
    class DefaultEuclideanGraphStorageTypeReference;
    template <class T, class GraphRepresentationTag, bool isDirected, class StorageTypeReference> class Graph;

    template <class T, int Dim, class Hasher = OriginalPerlinPermTable> class SimplexNoise;

    namespace { class OriginalPerlinPermTable; }
    template <int N, class IntType>
    class PermTable;
    template <class T, int Dim, class Hasher = OriginalPerlinPermTable> class PerlinNoise;
    template <class T> class CellNoise;

    template <int I> class RandomEngineT;

    template <class T> class RandomPointOnCircle2Picker;
    template <class T> class RandomPointOnLineSegment2Picker;
    template <class T> class RandomPointOnPolygonPicker;
    template <class T> class RandomPointOnPolyline2Picker;
    template <class T> class RandomPointOnRectangle2Picker;
    template <class T> class RandomPointOnTriangle2Picker;

    template <class T> class Raycast2;
    class Raycaster2;

    template <class T> class Angle2;
    template <class T> class Shape;
    template <class T> class Shape2;
    template <class T> class Shape3;
    template <class T> class Shape4;
    template <class T> class Vec2;
    template <class T> class Vec3;
    template <class T> class Vec4;
    template <class T, int C, int R> class Matrix;
    template <class T> class Rectangle2;
    template <class T> class Circle2;
    template <class T> class LineSegment2;
    template <class T> class LineSegment2View;
    template <class T> class Ray2;
    template <class T> class Triangle2;
    template <class T> class Triangle2View;
    template <class T> class Polygon2;
    template <class T> class Polygon2View;
    template <class T> class Polyline2;
    template <class T> class Polyline2View;
    template <class ShapeT> class ShapeMesh2;
    template <class T> class Cuboid3;
    template <class T> class Cylinder3;

    template <class T> class Transformation2;
    template <class T> class AffineTransformation2;
}
