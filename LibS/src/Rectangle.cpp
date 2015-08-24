template <class T>
Rectangle<T>::Rectangle(const Vec2<T>& p1, const Vec2<T>& p2) : min(p1), max(p2)
{
}
template <class T>
Rectangle<T>::Rectangle(const Vec2<T>& p1, const T width, const T height) : min(p1), max(p1 + Vec2<T>(width, height))
{
}
template <class T>
template <class X>
Rectangle<T>::Rectangle(const Rectangle<X>& r) : min(r.min), max(r.max)
{
}
template <class T>
template <class X>
Rectangle<T>& Rectangle<T>::operator =(const Rectangle<X>& r)
{
    min = r.min;
    max = r.max;
    return *this;
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
/*
template <class T>
Vec2<T> Rectangle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    T t1 = randomEngine.next<T>(T(0.0), T(1.0));
    T t2 = randomEngine.next<T>(T(0.0), T(1.0));
    return Vec2<T>(min + (max-min) * Vec2<T>(t1, t2));
}
template <class T>
Vec2<T> Rectangle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
*/
template <class T>
Vec2<T> Rectangle<T>::center() const
{
    return (min + max) / T(2);
}
template <class T>
T Rectangle<T>::area() const
{
    return width() * height();
}


template <class T>
Rectangle<T> Rectangle<T>::unitRectangle()
{
    return Rectangle<T>(Vec2<T> {T(0), T(0)}, T(1), T(1));
}
