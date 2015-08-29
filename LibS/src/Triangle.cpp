template <class T>
Triangle<T>::Triangle(const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3) : vertices{p1, p2, p3}
{
}
template <class T>
Triangle<T>::Triangle(const Vec2<T>* p) : vertices{p[0], p[1], p[2]}
{
}
template <class T>
Triangle<T> Triangle<T>::operator +(const Vec2<T>& p) const
{
    return Triangle<T>(vertices[0] + p, vertices[1] + p, vertices[2] + p);
}
template <class T>
Triangle<T>& Triangle<T>::operator +=(const Vec2<T>& p)
{
    vertices[0] += p;
    vertices[1] += p;
    vertices[2] += p;
    return *this;
}
template <class T>
Triangle<T> Triangle<T>::operator -(const Vec2<T>& p) const
{
    return Triangle<T>(vertices[0] - p, vertices[1] - p, vertices[2] - p);
}
template <class T>
Triangle<T>& Triangle<T>::operator -=(const Vec2<T>& p)
{
    vertices[0] -= p;
    vertices[1] -= p;
    vertices[2] -= p;
    return *this;
}

template <class T>
template <class T2>
Triangle<T>::operator Triangle<T2>() const
{
    return Triangle<T2>(static_cast<Vec2<T2>>(vertices[0]), static_cast<Vec2<T2>>(vertices[1]), static_cast<Vec2<T2>>(vertices[2]));
}

template <class T>
void Triangle<T>::translate(const Vec2<T>& v)
{
    vertices[0].translate(v);
    vertices[1].translate(v);
    vertices[2].translate(v);
}
template <class T>
void Triangle<T>::scale(const Vec2<T>& s)
{
    vertices[0].scale(s);
    vertices[1].scale(s);
    vertices[2].scale(s);
}

template <class T>
T Triangle<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Triangle<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(Intersections::intersection(point, *this)) return point;

    T minDistance = std::numeric_limits<T>::max();
    Vec2<T> nearestPoint(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());

    size_t size = 3;
    for(size_t i = 0; i < size; ++i)
    {
        const Vec2<T>& thisVertex = vertices[i];
        const Vec2<T>& nextVertex = vertices[(i + 1) % size];

        Vec2<T> nearPoint = LineSegment<T>(thisVertex, nextVertex).nearestPointTo(point);
        T distance = nearPoint.distanceTo(point);
        if(distance < minDistance)
        {
            minDistance = distance;
            nearestPoint = nearPoint;
        }
    }
    return nearestPoint;
}
template <class T>
Polyline<T> Triangle<T>::asPolyline() const
{
    return Polyline<T>(
    {
        vertices[0], vertices[1], vertices[2], vertices[0]
    });
}
template <class T>
Vec2<T> Triangle<T>::centerOfMass() const
{
    return (vertices[0] + vertices[1] + vertices[2]) / T(3);
}
template <class T>
Triangle<T> Triangle<T>::equilateral(const Vec2<T>& center, const T base)
{
    T height = base * static_cast<T>(SQRT3) / T(2);
    return Triangle<T>(Vec2<T> {center.x - base / T(2), center.y - height / T(2)},
        Vec2<T> {center.x, center.y + height / T(2)},
        Vec2<T> {center.x - base / T(2), center.y + height / T(2)});
}
template <class T>
Triangle<T> Triangle<T>::isosceles(const Vec2<T>& center, const T base, const T height)
{
    return Triangle<T>(Vec2<T> {center.x - base / T(2), center.y - height / T(2)},
        Vec2<T> {center.x, center.y + height / T(2)},
        Vec2<T> {center.x + base / T(2), center.y - height / T(2)});
}
template <class T>
Triangle<T> Triangle<T>::rightTriangle(const Vec2<T>& rightAngledVertex, const T width, const T height)
{
    return Triangle<T>(rightAngledVertex,
        rightAngledVertex + Vec2<T> {T(0), height},
        rightAngledVertex + Vec2<T> {width, T(0)});
}
template <class T>
T Triangle<T>::signedArea() const
{
    const Vec2<T>& a = vertices[0];
    const Vec2<T>& b = vertices[1];
    const Vec2<T>& c = vertices[2];
    return ((b.x - a.x) * (c.y - a.y) - (c.x - a.x) * (b.y - a.y)) / T(2);
}
template <class T>
T Triangle<T>::area() const
{
    return std::abs(signedArea());
}
