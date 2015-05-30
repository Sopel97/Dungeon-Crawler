template <class T>
Triangle<T>::Triangle(const Vec2<T>& p1, const Vec2<T>& p2, const Vec2<T>& p3) : vertices {p1, p2, p3}
{
}
template <class T>
Triangle<T>::Triangle(const Vec2<T>* p) : vertices {p[0], p[1], p[2]}
{
}
template <class T>
template <class X>
Triangle<T>::Triangle(const Triangle<X>& t) : vertices {t.vertices[0], t.vertices[1], t.vertices[2]}
{
}
template <class T>
template <class X>
Triangle<T>& Triangle<T>::operator =(const Triangle<X>& t)
{
    vertices[0] = t.vertices[0];
    vertices[1] = t.vertices[1];
    vertices[2] = t.vertices[2];
    return *this;
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
void Triangle<T>::transform(const std::function<void(Vec2<T>&)>& transformationFunction)
{
    transformationFunction(vertices[0]);
    transformationFunction(vertices[1]);
    transformationFunction(vertices[2]);
}
template <class T>
void Triangle<T>::transform(const Transformation2<T>& transformation)
{
    transformation.transform(vertices[0]);
    transformation.transform(vertices[1]);
    transformation.transform(vertices[2]);
}
template <class T>
Triangle<T> Triangle<T>::transformed(const std::function<void(Vec2<T>&)>& transformationFunction) const
{
    Triangle<T> copy(*this);
    copy.transform(transformationFunction);
    return copy;
}
template <class T>
Triangle<T> Triangle<T>::transformed(const Transformation2<T>& transformation) const
{
    Triangle<T> copy(*this);
    copy.transform(transformation);
    return copy;
}

template <class T>
T Triangle<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Triangle<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(point.intersects(*this)) return point;

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
Vec2<T> Triangle<T>::center() const
{
    return (vertices[0] + vertices[1] + vertices[2]) / 3.0;
}

template <class T>
std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> Triangle<T>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<Triangle<T>::RandomPointPickerPreprocessedData>
           (
               vertices[2] - vertices[1],
               vertices[2] - vertices[0],
               vertices[1] - vertices[0],
               vertices[0] - vertices[1]
           );
}
template <class T>
Vec2<T> Triangle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine) const
{
    T t1 = randomEngine.nextDouble();
    T t2 = randomEngine.nextDouble();
    Vec2<T> point(vertices[0] + (vertices[2] - vertices[0]) * t1 + (vertices[1] - vertices[0]) * t2);
    if
    (
        ((vertices[2].x - vertices[1].x) * (point.y - vertices[1].y) - (vertices[2].y - vertices[1].y) * (point.x - vertices[1].x))
        *
        ((vertices[2].x - vertices[1].x) * (vertices[0].y - vertices[1].y) - (vertices[2].y - vertices[1].y) * (vertices[0].x - vertices[1].x))
        <
        0.0
    ) //points vertices[0] and point are on the opposite sides of the edge
    {
        Vec2<T> edgeCenter = vertices[1] + (vertices[2] - vertices[1]) / 2.0;
        point = edgeCenter * 2.0 - point;
    }
    return point;
}

template <class T>
Vec2<T> Triangle<T>::pickRandomPoint(Random::RandomEngineBase& randomEngine, typename Shape2<T>::RandomPointPickerPreprocessedData& preprocessedData) const
{
    Triangle<T>::RandomPointPickerPreprocessedData& trianglePreprocessedData = static_cast<Triangle<T>::RandomPointPickerPreprocessedData&>(preprocessedData);
    T t1 = randomEngine.nextDouble();
    T t2 = randomEngine.nextDouble();
    Vec2<T> point(vertices[0] + trianglePreprocessedData.edge02 * t1 + trianglePreprocessedData.edge01 * t2);
    if
    (
        (trianglePreprocessedData.edge12.x * (point.y - vertices[1].y) - trianglePreprocessedData.edge12.y * (point.x - vertices[1].x))
        *
        (trianglePreprocessedData.edge12.x * trianglePreprocessedData.edge10.y - trianglePreprocessedData.edge12.y * trianglePreprocessedData.edge10.x)
        <
        0.0
    ) //points vertices[0] and point are on the opposite sides of the edge
    {
        Vec2<T> edgeCenter = vertices[1] + trianglePreprocessedData.edge12 / 2.0;
        point = edgeCenter * 2.0 - point;
    }
    return point;
    return pickRandomPoint(randomEngine);
}


template <class T>
Triangle<T> Triangle<T>::equilateral(const Vec2D& center, const T base)
{
    T height = base * std::sqrt(3) * 0.5;
    return Triangle<T>(Vec2D {center.x - base * 0.5, center.y - height * 0.5},
                       Vec2D {center.x, center.y + height * 0.5},
                       Vec2D {center.x - base * 0.5, center.y + height * 0.5});
}
template <class T>
Triangle<T> Triangle<T>::isosceles(const Vec2D& center, const T base, const T height)
{
    return Triangle<T>(Vec2D {center.x - base * 0.5, center.y - height * 0.5},
                       Vec2D {center.x, center.y + height * 0.5},
                       Vec2D {center.x + base * 0.5, center.y - height * 0.5});
}
template <class T>
Triangle<T> Triangle<T>::rightTriangle(const Vec2D& rightAngledVertex, const T width, const T height)
{
    return Triangle<T>(rightAngledVertex,
                       rightAngledVertex + Vec2D {0.0, height},
                       rightAngledVertex + Vec2D {width, 0.0});
}
template <class T>
std::unique_ptr<Shape2<T>> Triangle<T>::clone() const
{
    return std::make_unique<Triangle<T>>(*this);
}
template <class T>
T Triangle<T>::signedArea() const
{
    const Vec2<T>& a = vertices[0];
    const Vec2<T>& b = vertices[1];
    const Vec2<T>& c = vertices[2];
    return ((b.x-a.x)*(c.y-a.y) - (c.x-a.x)*(b.y-a.y)) / 2.0;
}
