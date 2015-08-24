template <class T>
RandomPointOnCirclePicker<T>::RandomPointOnCirclePicker(const Circle<T>& circle) :
    m_circle(circle)
{
}
template <class T>
template <class RNG>
Vec2<T> RandomPointOnCirclePicker<T>::nextPoint(RNG& rng) const
{
    T r = std::sqrt(rng.next(T(0), T(1))) * m_circle.radius;
    AngleD angle = AngleD::radians(rng.next(T(0), T(2) * PI));
    return m_circle.origin + Vec2<T>(r * angle.cos(), r * angle.sin());
}

template <class T>
RandomPointOnLineSegmentPicker<T>::RandomPointOnLineSegmentPicker(const LineSegment<T>& lineSegment) :
    m_lineSegment(lineSegment)
{

}
template <class T>
template <class RNG>
Vec2<T> RandomPointOnLineSegmentPicker<T>::nextPoint(RNG& rng) const
{
    T t = rng.next(T(0), T(1));
    return m_lineSegment.begin + (m_lineSegment.end - m_lineSegment.begin) * t;
}

template <class T>
RandomPointOnPolygonPicker<T>::RandomPointOnPolygonPicker(const Polygon<T>& polygon) :
    m_polygon(polygon),
    m_triangulation(m_polygon)
{
    m_trianglesByArea.reserve(m_triangulation.triangleMesh().size());
    for(auto& triangle : m_triangulation.triangleMesh().elements)
    {
        m_trianglesByArea.push_back(std::make_pair(&triangle, triangle.area()));
    }
    size_t numberOfTriangles = m_trianglesByArea.size();
    for(size_t i = 1; i < numberOfTriangles; ++i)
    {
        m_trianglesByArea[i].second += m_trianglesByArea[i - 1].second; //so the area is cumulative
    }
}
template <class T>
template <class RNG>
Vec2<T> RandomPointOnPolygonPicker<T>::nextPoint(RNG& rng) const
{
    T sumOfAreas = m_trianglesByArea.back().second;
    T randomArea = rng.next(T(0), sumOfAreas);
    auto chosenTriangleIter = std::upper_bound(m_trianglesByArea.begin(), m_trianglesByArea.end(), randomArea, [](const T& lhs, const std::pair<const Triangle<T>*, T>& rhs)->bool{return lhs < rhs.second;});
    if(chosenTriangleIter == m_trianglesByArea.end()) chosenTriangleIter = m_trianglesByArea.begin();
    return RandomPointOnTrianglePicker<T>(*(chosenTriangleIter->first)).nextPoint(rng);
}

template <class T>
RandomPointOnPolylinePicker<T>::RandomPointOnPolylinePicker(const Polyline<T>& polyline) :
    m_polyline(polyline)
{

}
template <class T>
template <class RNG>
Vec2<T> RandomPointOnPolylinePicker<T>::nextPoint(RNG& rng) const
{

}

template <class T>
RandomPointOnRectanglePicker<T>::RandomPointOnRectanglePicker(const Rectangle<T>& rectangle) :
    m_rectangle(rectangle)
{

}
template <class T>
template <class RNG>
Vec2<T> RandomPointOnRectanglePicker<T>::nextPoint(RNG& rng) const
{
    T t1 = rng.next(T(0), T(1));
    T t2 = rng.next(T(0), T(1));
    return Vec2<T>(m_rectangle.min + (m_rectangle.max-m_rectangle.min) * Vec2<T>(t1, t2));
}

template <class T>
RandomPointOnTrianglePicker<T>::RandomPointOnTrianglePicker(const Triangle<T>& triangle) :
    m_triangle(triangle)
{

}
template <class T>
template <class RNG>
Vec2<T> RandomPointOnTrianglePicker<T>::nextPoint(RNG& rng) const
{
    T t1 = rng.next(T(0), T(1));
    T t2 = rng.next(T(0), T(1));
    Vec2<T> point(m_triangle.vertices[0] + (m_triangle.vertices[2] - m_triangle.vertices[0]) * t1 + (m_triangle.vertices[1] - m_triangle.vertices[0]) * t2);
    if
    (
        ((m_triangle.vertices[2].x - m_triangle.vertices[1].x) * (point.y - m_triangle.vertices[1].y) - (m_triangle.vertices[2].y - m_triangle.vertices[1].y) * (point.x - m_triangle.vertices[1].x))
        *
        ((m_triangle.vertices[2].x - m_triangle.vertices[1].x) * (m_triangle.vertices[0].y - m_triangle.vertices[1].y) - (m_triangle.vertices[2].y - m_triangle.vertices[1].y) * (m_triangle.vertices[0].x - m_triangle.vertices[1].x))
        <
        T(0)
    ) //points vertices[0] and point are on the opposite sides of the edge
    {
        Vec2<T> edgeCenter = m_triangle.vertices[1] + (m_triangle.vertices[2] - m_triangle.vertices[1]) / T(2);
        point = edgeCenter * T(2) - point;
    }
    return point;
}
