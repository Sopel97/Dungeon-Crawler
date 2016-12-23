#pragma once

#include "..\Fwd.h"

#include "Vec2.h"
#include "Ray2.h"

#include <vector>

namespace ls
{
    template <class T>
    class Raycast2
    {
        static_assert(std::is_floating_point<T>::value, "T must be of floating-point type");
    public:
        using ValueType = T;

        struct Hit
        {
            T distance;
            Vec2<T> normal;
            Vec2<T> hitPoint;
        };

        const Ray2<T> ray; //read-only

        Raycast2() = default;
        Raycast2(const Ray2<T>& _ray) noexcept(std::is_nothrow_copy_constructible<Ray2<T>>::value);
        Raycast2(const Ray2<T>& _ray, const std::vector<Hit>& _hits);
        Raycast2(const Ray2<T>& _ray, std::vector<Hit>&& _hits) noexcept(std::is_nothrow_copy_constructible<Ray2<T>>::value && std::is_nothrow_move_constructible<std::vector<Hit>>::value);

        Raycast2(const Raycast2<T>&) = default;
        Raycast2(Raycast2<T>&&) noexcept = default;

        Raycast2<T>& operator = (const Raycast2<T>&) & = default;
        Raycast2<T>& operator = (Raycast2<T> &&) & noexcept = default;

        int numberOfHits() const;

        const std::vector<Hit>& hits() const &;
        std::vector<Hit> hits() &&;

        void sortHitPointsByDistance() &;
    protected:
        std::vector<Hit> m_hits;
    };

    using Raycast2D = Raycast2<double>;
    using Raycast2F = Raycast2<float>;

    extern template class Raycast2<double>;
    extern template class Raycast2<float>;
}
#include "../src/Raycast2.cpp"
