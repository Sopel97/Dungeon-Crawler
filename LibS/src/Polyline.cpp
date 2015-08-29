template <class T>
Polyline<T>::Polyline(const std::initializer_list<Vec2<T>>& list) : vertices(list)
{
}
template <class T>
Polyline<T>::Polyline(const std::vector<Vec2<T>>& v) : vertices(v)
{
}
template <class T>
Polyline<T>::Polyline(std::vector<Vec2<T>>&& v) : vertices(std::move(v))
{
}
template <class T>
Polyline<T>::Polyline(Vec2<T>* v, size_t count)
{
    vertices.assign(v, v + count);
}

template <class T>
Polyline<T> Polyline<T>::operator+(const Vec2<T>& v) const
{
    Polyline newPolyline(*this);
    newPolyline += v;
    return newPolyline;
}
template <class T>
Polyline<T>& Polyline<T>::operator+=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex += v;
    }
    return *this;
}
template <class T>
Polyline<T> Polyline<T>::operator-(const Vec2<T>& v) const
{
    Polyline newPolyline(*this);
    newPolyline -= v;
    return newPolyline;
}
template <class T>
Polyline<T>& Polyline<T>::operator-=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex -= v;
    }
    return *this;
}

template <class T>
template <class T2>
Polyline<T>::operator Polyline<T2>() const
{
    Polyline<T2> poly;
    poly.reserve(vertices.size());
    for(const auto& v : vertices)
    {
        poly.push_back(static_cast<Vec2<T2>>(v));
    }
    return poly;
}

template <class T>
void Polyline<T>::add(const Vec2<T>& v)
{
    vertices.push_back(v);
}
template <class T>
void Polyline<T>::add(const std::vector<T>& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}
template <class T>
void Polyline<T>::add(const std::initializer_list<Vec2<T>>& list)
{
    vertices.insert(vertices.end(), list.begin(), list.end());
}

template <class T>
void Polyline<T>::add(Vec2<T>&& v)
{
    vertices.push_back(v);
}
template <class T>
void Polyline<T>::add(std::vector<T>&& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}

template <class T>
void Polyline<T>::translate(const Vec2<T>& v)
{
    for(auto& vert : vertices)
    {
        vert.translate(v);
    }
}
template <class T>
void Polyline<T>::scale(const Vec2<T>& s)
{
    for(auto& vert : vertices)
    {
        vert.scale(s);
    }
}
template <class T>
Vec2<T> Polyline<T>::project(const Vec2<T>& b) const
{
    Vec2<T> projection;

    return projection;
}

template <class T>
std::pair<T, T> Polyline<T>::projectMinMax(const Vec2<T>& b) const
{
    T minProjection;
    T maxProjection;
    minProjection = maxProjection = vertices[0].dot(b);
    for(const Vec2<T>& vertex : vertices)
    {
        T dotProduct = vertex.dot(b);
        if(dotProduct < minProjection) minProjection = dotProduct;
        else if(dotProduct > maxProjection) maxProjection = dotProduct;
    }
    return std::pair<T, T>(minProjection, maxProjection);
}
/*
template <class T>
std::unique_ptr<typename Shape2<T>::RandomPointPickerPreprocessedData> Polyline<T>::createPreprocessedDataForRandomPointPicker() const
{
    return std::make_unique<RandomPointPickerPreprocessedData>();
}
*/
template <class T>
T Polyline<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Polyline<T>::nearestPointTo(const Vec2<T>& point) const
{
    T minDistance = std::numeric_limits<T>::max();
    Vec2<T> nearestPoint(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
    size_t polySize = size();
    for(size_t i = 0; i < polySize - 1; ++i)
    {
        const Vec2<T>& thisVertex = vertices[i];
        const Vec2<T>& nextVertex = vertices[i + 1];

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
size_t Polyline<T>::size() const
{
    return vertices.size();
}


template <class T>
Polyline<T> Polyline<T>::fromRectangle(const Rectangle<T>& rectangle)
{
    return Polyline<T> {Vec2<T>{rectangle.min.x, rectangle.min.y},
        Vec2<T>{rectangle.max.x, rectangle.min.y},
        Vec2<T>{rectangle.max.x, rectangle.max.y},
        Vec2<T>{rectangle.min.x, rectangle.max.y},
        Vec2<T>{rectangle.min.x, rectangle.min.y}
    };
}
template <class T>
Polyline<T> Polyline<T>::fromTriangle(const Triangle<T>& triangle)
{
    return Polyline<T> {triangle.vertices[0], triangle.vertices[1], triangle.vertices[2], triangle.vertices[0]};
}
