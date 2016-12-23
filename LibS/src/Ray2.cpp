#include "..\include\Ray2.h"

namespace ls
{
    template <class T>
    constexpr Ray2<T>::Ray2(const Vec2<T>& _origin, const Vec2<T>& _direction) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
        origin(_origin),
        direction(_direction.normalized())
    {

    }

    template <class T>
    constexpr Ray2<T> Ray2<T>::translated(const Vec2<T>& displacement) const
    {
        return Ray2<T>(origin + displacement, direction);
    }
    template <class T>
    Ray2<T>& Ray2<T>::translate(const Vec2<T>& displacement) &
    {
        origin += displacement;
        return *this;
    }

    template <class T>
    template <class T2>
    Ray2<T>::operator Ray2<T2>() const
    {
        return Ray2<T2>(static_cast<Vec2<T2>>(origin), static_cast<Vec2<T2>>(direction));
    }

    template <class T>
    constexpr bool operator== (const Ray2<T>& lhs, const Ray2<T>& rhs)
    {
        return std::tie(lhs.origin, lhs.direction) == std::tie(rhs.origin, rhs.direction);
    }
    template <class T>
    constexpr bool operator!= (const Ray2<T>& lhs, const Ray2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}
