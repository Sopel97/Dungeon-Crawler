template <class T>
Circle<T>::Circle(const Vec2<T>& p1, T r) : origin(p1), radius(r)
{
}
template <class T>
template <class X>
Circle<T>::Circle(const Circle<X>& c) : origin(c.origin), radius(c.radius)
{
}
template <class T>
template <class X>
Circle<T>& Circle<T>::operator =(const Circle<X>& c)
{
    origin = c.origin;
    radius = c.radius;
    return *this;
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
void Circle<T>::transform(const std::function<void(Vec2<T>&)>& transformationFunction)
{
    Vec2<T> somePointOnCircle = origin + Vec2<T>(radius, 0);
    transformationFunction(somePointOnCircle);

    transformationFunction(origin);
    radius = origin.distanceTo(somePointOnCircle);
}
template <class T>
void Circle<T>::transform(const Transformation2<T>& transformation)
{
    Vec2<T> somePointOnCircle = origin + Vec2<T>(radius, 0);
    transformation.transform(somePointOnCircle);

    transformation.transform(origin);
    radius = origin.distanceTo(somePointOnCircle);
}
template <class T>
Circle<T> Circle<T>::transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const
{
    Circle<T> copy(*this);
    copy.transform(transformationFunction);
    return copy;
}
template <class T>
Circle<T> Circle<T>::transformed(const Transformation2<T>& transformation) const
{
    Circle<T> copy(*this);
    copy.transform(transformation);
    return copy;
}

template <class T>
T Circle<T>::distanceTo(const Vec2<T>& v1) const
{
    return std::max(T(0.0), v1.distanceTo(origin) - radius);
}
template <class T>
Vec2<T> Circle<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(point.intersects(*this)) return point;

    return (point - origin).normalized() * radius;
}
template <class T>
Polyline<T> Circle<T>::asPolyline() const
{
    const int segmentCount = 64;
    Polyline<T> out;
    float angle = 0;
    float angleDiff = 2 * PI / (float)segmentCount;
    for(int i = 0; i < segmentCount; ++i)
    {
        angle += angleDiff;
        Vec2<T> next(cos(angle)*radius + origin.x, sin(angle)*radius + origin.y);
        out.add(next);
    }
    out.add(out.vertices[0]);
    return out;
}

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
template <class T>
Vec2<T> Circle<T>::center() const
{
    return origin;
}

template <class T>
T Circle<T>::signedArea() const
{
    return PI*radius*radius;
}
template <class T>
std::unique_ptr<Shape2<T>> Circle<T>::clone() const
{
    return std::make_unique<Circle<T>>(*this);
}
