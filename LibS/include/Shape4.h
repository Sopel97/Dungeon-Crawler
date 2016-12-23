#pragma once

#include "..\Fwd.h"

namespace ls
{
    template <class T>
    class Shape4 : public Shape<T>
    {
    public:
        using ValueType = T;

        using VectorType = Vec4<T>;
    };

    using Shape4D = Shape4<double>;
    using Shape4F = Shape4<float>;
    using Shape4I = Shape4<int>;

    extern template class Shape4<double>;
    extern template class Shape4<float>;
    extern template class Shape4<int>;
}
#include "../src/Shape4.cpp"
