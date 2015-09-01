template <class T>
Polygon<T>::Polygon(const std::initializer_list<Vec2<T>>& list) : vertices(list)
{
}
template <class T>
Polygon<T>::Polygon(const std::vector<Vec2<T>>& v) : vertices(v)
{
}
template <class T>
Polygon<T>::Polygon(std::vector<Vec2<T>>&& v) : vertices(std::move(v))
{
}
template <class T>
Polygon<T>::Polygon(Vec2<T>* v, size_t count)
{
    vertices.assign(v, v + count);
}
template <class T>
Polygon<T> Polygon<T>::operator+(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon += v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator+=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex += v;
    }
    return *this;
}
template <class T>
Polygon<T> Polygon<T>::operator-(const Vec2<T>& v) const
{
    Polygon newPolygon(*this);
    newPolygon -= v;
    return newPolygon;
}
template <class T>
Polygon<T>& Polygon<T>::operator-=(const Vec2<T>& v)
{
    for(Vec2<T>& vertex : vertices)
    {
        vertex -= v;
    }
    return *this;
}

template <class T>
template <class T2>
Polygon<T>::operator Polygon<T2>() const
{
    Polygon<T2> poly;
    poly.reserve(vertices.size());
    for(const auto& v : vertices)
    {
        poly.push_back(static_cast<Vec2<T2>>(v));
    }
    return poly;
}

template <class T>
void Polygon<T>::add(const Vec2<T>& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(const std::vector<T>& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}
template <class T>
void Polygon<T>::add(const std::initializer_list<Vec2<T>>& list)
{
    vertices.insert(vertices.end(), list.begin(), list.end());
}

template <class T>
void Polygon<T>::add(Vec2<T>&& v)
{
    vertices.push_back(v);
}
template <class T>
void Polygon<T>::add(std::vector<T>&& v)
{
    vertices.insert(vertices.end(), v.begin(), v.end());
}


template <class T>
std::pair<T, T> Polygon<T>::projectMinMax(const Vec2<T>& b) const
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
template <class T>
T Polygon<T>::distanceTo(const Vec2<T>& point) const
{
    return point.distanceTo(nearestPointTo(point));
}
template <class T>
Vec2<T> Polygon<T>::nearestPointTo(const Vec2<T>& point) const
{
    if(Intersections::intersection(point, *this)) return point;

    T minDistance = std::numeric_limits<T>::max();
    Vec2<T> nearestPoint(std::numeric_limits<T>::max(), std::numeric_limits<T>::max());
    size_t polySize = size();
    for(size_t i = 0; i < polySize; ++i)
    {
        const Vec2<T>& thisVertex = vertices[i];
        const Vec2<T>& nextVertex = vertices[(i + 1) % polySize];

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
Polyline<T> Polygon<T>::asPolyline() const
{
    Polyline<T> out(vertices);
    out.add(out.vertices[0]);
    return out;
}
template <class T>
Vec2<T> Polygon<T>::centerOfMass() const
{
    T xsum = T(0);
    T ysum = T(0);
    T area = T(0);
    for(size_t i = 0; i < size(); ++i)
    {
        const Vec2<T>& p0 = vertices[i];
        const Vec2<T>& p1 = vertices[(i + 1) % size()];

        T areaSum = (p0.x * p1.y) - (p1.x * p0.y);

        xsum += (p0.x + p1.x) * areaSum;
        ysum += (p0.y + p1.y) * areaSum;

        area += areaSum;
    }

    T centMassX = xsum / (area * T(3));
    T centMassY = ysum / (area * T(3));

    return Vec2<T>(centMassX, centMassY);
}
template <class T>
bool Polygon<T>::isConvex() const
{
    size_t numberOfVertices = vertices.size();
    T prevCross = 0; //will be initialized by correct cross when i equals 0 in the loop
    for(size_t i = 0; i < numberOfVertices; ++i)
    {
        const Vec2<T>& v0 = vertices[i];
        const Vec2<T>& v1 = vertices[(i + 1)&numberOfVertices];
        const Vec2<T>& v2 = vertices[(i + 2)&numberOfVertices];

        Vec2<T> e0 = v1 - v0;
        Vec2<T> e1 = v2 - v1;

        T cross = e0.cross(e1);
        if(i != 0)
        {
            if(cross * prevCross < T(0)) return false; //if cross and prev cross have different signs
        }
        prevCross = cross;
    }
    return true;
}
template <class T>
T Polygon<T>::signedArea() const
{
    T area = 0;
    for(size_t i = 0; i < size(); ++i)
    {
        const Vec2<T>& p0 = vertices[i];
        const Vec2<T>& p1 = vertices[(i + 1) % size()];

        area += (p0.x * p1.y) - (p1.x * p0.y);
    }
    return area / T(2);
}
template <class T>
T Polygon<T>::area() const
{
    return std::abs(signedArea());
}

template <class T>
size_t Polygon<T>::size() const
{
    return vertices.size();
}

template <class T>
Polygon<T> Polygon<T>::regular(const Vec2<T>& center, int sides, T radius)
{
    Polygon<T> polygon;
    polygon.vertices.reserve(sides);
    for(int i = 0; i < sides; ++i)
    {
        polygon.vertices.emplace_back(center.x + radius * static_cast<T>(std::cos(T(2) * static_cast<T>(PI) * static_cast<T>(i) / static_cast<T>(sides))), center.y + radius * static_cast<T>(std::sin(T(2) * static_cast<T>(PI) * static_cast<T>(i) / static_cast<T>(sides))));
    }
    return polygon;
}
template <class T>
Polygon<T> Polygon<T>::fromRectangle(const Rectangle<T>& rectangle)
{
    return Polygon<T> {Vec2<T>{rectangle.min.x, rectangle.min.y},
        Vec2<T>{rectangle.max.x, rectangle.min.y},
        Vec2<T>{rectangle.max.x, rectangle.max.y},
        Vec2<T>{rectangle.min.x, rectangle.max.y}
    };
}
template <class T>
Polygon<T> Polygon<T>::fromTriangle(const Triangle<T>& triangle)
{
    return Polygon<T> {triangle.vertices[0], triangle.vertices[1], triangle.vertices[2]};
}
