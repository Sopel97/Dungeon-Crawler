#ifndef SHAPES_H_INCLUDED
#define SHAPES_H_INCLUDED

#include <vector>
#include <set>
#include <cmath>
#include <utility>
#include <functional>
#include <algorithm>
#include <array>
#include "Array2.h"

#include "Constants.h"
//maybe will be renamed to Shapes and some functions will be extracted to other files
//NOTE: 3d shapes are leftover from early stages of the library and will be completly rewritten later.
namespace ls
{
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

}
#endif // SHAPES_H_INCLUDED
