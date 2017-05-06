#pragma once

#include "..\Fwd.h"

#include "Shape.h"

/*  Available 2D shapes

    Circle2
    LineSegment2
    Polygon2
    Polyline2
    Ray2
    Rectangle2
    Triangle2
    Vec2

*/

namespace ls
{
    template <class T>
    class Shape2 : public Shape<T>
    {
    public:
        using ValueType = T;

        using VectorType = Vec2<T>;

        constexpr Shape2() = default;
    };

    using Shape2D = Shape2<double>;
    using Shape2F = Shape2<float>;
    using Shape2I = Shape2<int>;

    extern template class Shape2<double>;
    extern template class Shape2<float>;
    extern template class Shape2<int>;
}
#include "../src/Shape2.cpp"
