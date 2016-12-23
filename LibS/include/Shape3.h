#pragma once

#include "..\Fwd.h"

namespace ls
{
    template <class T>
    class Shape3 : public Shape<T>
    {
    public:
        using ValueType = T;

        using VectorType = Vec3<T>;
    };

    using Shape3D = Shape3<double>;
    using Shape3F = Shape3<float>;
    using Shape3I = Shape3<int>;

    extern template class Shape3<double>;
    extern template class Shape3<float>;
    extern template class Shape3<int>;
}
#include "../src/Shape3.cpp"
