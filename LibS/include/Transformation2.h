#pragma once

#include "..\Fwd.h"

#include "Vec2.h"

namespace ls
{
    template <class T>
    class Transformation2
    {
    public:
        using ValueType = T;

        Transformation2() noexcept = default;

        virtual void transform(Vec2<T>& vector) const = 0;
        virtual Vec2<T> transformed(const Vec2<T>& vector) const = 0;
    protected:
    };

    using Transformation2F = Transformation2<float>;
    using Transformation2D = Transformation2<double>;

    extern template class Transformation2<double>;
    extern template class Transformation2<float>;
}
#include "../src/Transformation2.cpp"
