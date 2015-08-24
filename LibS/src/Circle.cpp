template <class T>
Circle<T>::Circle(const Vec2<T>& p1, T r) : origin(p1), radius(r)
{
}
template <class T>
Circle<T> Circle<T>::operator +(const Vec2<T>& v) const
{
    return Circle<T>(origin + v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator +=(const Vec2<T>& v)
{
    origin += v;
    return *this;
}
template <class T>
Circle<T> Circle<T>::operator -(const Vec2<T>& v) const
{
    return Circle<T>(origin - v, radius);
}
template <class T>
Circle<T>& Circle<T>::operator -=(const Vec2<T>& v)
{
    origin -= v;
    return *this;
}

template <class T>
void Circle<T>::translate(const Vec2<T>& v)
{
    origin.translate(v);
}
template <class T>
void Circle<T>::scale(const Vec2<T>& s) //undefined when s.x != s.y
{
    radius *= s.x;
    origin.scale(s);
}
template <class T>
T Circle<T>::distanceTo(const Vec2<T>& v1) const
{
    return std::max(T(0.0), v1.distanceTo(origin) - radius);
}
template <class T>
Vec2<T> Circle<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(Intersections::intersection(point, *this)) return point;

    return (point - origin).normalized() * radius;
}
/*
template <class T>
Vec2<T> Circle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    T r = std::sqrt(randomEngine.next<T>(T(0.0), T(1.0))) * radius;
    AngleD angle = AngleD::radians(randomEngine.next<T>(T(0.0), T(2.0) * PI));
    return origin + Vec2<T>(r * angle.cos(), r * angle.sin());
}
template <class T>
Vec2<T> Circle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    return pickRandomPoint(randomEngine);
}
*/
template <class T>
Vec2<T> Circle<T>::center() const
{
    return origin;
}
