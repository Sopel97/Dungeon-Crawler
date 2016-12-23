#include "..\include\Circle2.h"

namespace ls
{
    template <class T>
    constexpr Circle2<T>::Circle2(const Vec2<T>& _origin, const T& _radius) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value && std::is_nothrow_copy_constructible<T>::value) :
        origin(_origin), 
        radius(_radius)
    {
    }
    
    template <class T>
    constexpr Circle2<T> Circle2<T>::translated(const Vec2<T>& displacement) const
    {
        return Circle2<T>(origin + displacement, radius);
    }
    template <class T>
    Circle2<T>& Circle2<T>::translate(const Vec2<T>& displacement) &
    {
        origin += displacement;
        return *this;
    }
    template <class T>
    Rectangle2<T> Circle2<T>::boundingBox() const
    {
        const Vec2<T> halfDiagonal(radius, radius);
        return Rectangle2<T>(origin - halfDiagonal, origin + halfDiagonal);
    }

    template <class T>
    template <class T2>
    Circle2<T>::operator Circle2<T2>() const
    {
        return Circle2<T2>(static_cast<Vec2<T2>>(origin), static_cast<T2>(radius));
    }

    template <class T>
    constexpr Vec2<T> Circle2<T>::centerOfMass() const
    {
        return origin;
    }

    template <class T>
    constexpr T Circle2<T>::area() const
    {
        return radius*radius*static_cast<T>(PI);
    }

    template <class T>
    constexpr bool operator==(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        return std::tie(lhs.origin, lhs.radius) == std::tie(rhs.origin, rhs.radius);
    }
    template <class T>
    constexpr bool operator!=(const Circle2<T>& lhs, const Circle2<T>& rhs)
    {
        return !(lhs == rhs);
    }

}
