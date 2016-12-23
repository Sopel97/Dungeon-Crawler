#include "..\include\Rectangle2.h"

namespace ls
{
    template <class T>
    constexpr Rectangle2<T>::Rectangle2(const Vec2<T>& _min, const Vec2<T>& _max) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value) :
        min(_min),
        max(_max)
    {
    }

    template <class T>
    constexpr Rectangle2<T> Rectangle2<T>::translated(const Vec2<T>& displacement) const
    {
        return Rectangle2<T>(min + displacement, max + displacement);
    }
    template <class T>
    Rectangle2<T>& Rectangle2<T>::translate(const Vec2<T>& displacement) &
    {
        min += displacement;
        max += displacement;
        return *this;
    }

    template <class T>
    template <class T2>
    Rectangle2<T>::operator Rectangle2<T2>() const
    {
        return Rectangle2<T2>(static_cast<Vec2<T2>>(min), static_cast<Vec2<T2>>(max));
    }

    template <class T>
    constexpr T Rectangle2<T>::width() const
    {
        return max.x - min.x;
    }
    template <class T>
    constexpr T Rectangle2<T>::height() const
    {
        return max.y - min.y;
    }

    template <class T>
    constexpr Vec2<T> Rectangle2<T>::centerOfMass() const
    {
        return (min + max) * T(0.5);
    }
    template <class T>
    constexpr T Rectangle2<T>::area() const
    {
        return width()*height();
    }

    template <class T>
    Rectangle2<T> Rectangle2<T>::withSize(const Vec2<T>& min, const T& width, const T& height) noexcept(std::is_nothrow_copy_constructible<Vec2<T>>::value)
    {
        return Rectangle2<T>(min, min + Vec2<T>(width, height));
    }
    template <class T>
    Rectangle2<T> Rectangle2<T>::unitRectangle() noexcept(std::is_nothrow_constructible<Rectangle2<T>, const Vec2<T>&, const Vec2<T>&>::value)
    {
        return Rectangle2<T>(Vec2<T>(T(0), T(0)), Vec2<T>(T(1), T(1)));
    }

    template <class T>
    constexpr bool operator== (const Rectangle2<T>& lhs, const Rectangle2<T>& rhs)
    {
        return std::tie(lhs.min, lhs.max) == std::tie(rhs.min, rhs.max);
    }
    template <class T>
    constexpr bool operator!= (const Rectangle2<T>& lhs, const Rectangle2<T>& rhs)
    {
        return !(lhs == rhs);
    }
}