template <class T>
Rectangle<T>::Rectangle(const Vec2<T>& p1, const Vec2<T>& p2) : min(p1), max(p2)
{
}
template <class T>
Rectangle<T>::Rectangle(const Vec2<T>& p1, const T width, const T height) : min(p1), max(p1 + Vec2<T>(width, height))
{
}

template <class T>
Rectangle<T> Rectangle<T>::operator +(const Vec2<T>& v) const
{
    return Rectangle<T>(min + v, max + v);
}
template <class T>
Rectangle<T>& Rectangle<T>::operator +=(const Vec2<T>& v)
{
    min += v;
    max += v;
    return *this;
}
template <class T>
Rectangle<T> Rectangle<T>::operator -(const Vec2<T>& v) const
{
    return Rectangle<T>(min - v, max - v);
}
template <class T>
Rectangle<T>& Rectangle<T>::operator -=(const Vec2<T>& v)
{
    min -= v;
    max -= v;
    return *this;
}

template <class T>
    template <class T2>
    Rectangle<T>::operator Rectangle<T2>() const
    {
        return Rectangle<T2>(static_cast<Vec2<T2>>(min), static_cast<Vec2<T2>>(max));
    }

template <class T>
T Rectangle<T>::width() const
{
    return max.x - min.x;
}
template <class T>
T Rectangle<T>::height() const
{
    return max.y - min.y;
}

template <class T>
void Rectangle<T>::translate(const Vec2<T>& v)
{
    min.translate(v);
    max.translate(v);
}
template <class T>
void Rectangle<T>::scale(const Vec2<T>& s)
{
    min.scale(s);
    max.scale(s);
}
template <class T>
T Rectangle<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Rectangle<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(Intersections::intersection(point, *this)) return point;

    return Vec2<T>(clamp(point.x, min.x, max.x), clamp(point.y, min.y, max.y));
}
template <class T>
Polyline<T> Rectangle<T>::asPolyline() const
{
    return Polyline<T>(
    {
        min,
        Vec2<T>(max.x, min.y),
        max,
        Vec2<T>(min.x, max.y),
        min
    }
    );
}
template <class T>
Vec2<T> Rectangle<T>::centerOfMass() const
{
    return (min + max) / T(2);
}
template <class T>
T Rectangle<T>::area() const
{
    return width()*height();
}


template <class T>
Rectangle<T> Rectangle<T>::unitRectangle()
{
    return Rectangle<T>(Vec2<T> {T(0), T(0)}, T(1), T(1));
}
