#pragma once

#include "..\Fwd.h"

namespace ls
{
    template <class T>
    class Shape
    {
    public:
        using ValueType = T;

        constexpr Shape() = default;
    };

    using ShapeD = Shape<double>;
    using ShapeF = Shape<float>;
    using ShapeI = Shape<int>;

    extern template class Shape<double>;
    extern template class Shape<float>;
    extern template class Shape<int>;
}
#include "../src/Shape.cpp"
