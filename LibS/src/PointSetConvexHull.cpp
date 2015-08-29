template <class T>
PointSetConvexHull<T>::PointSetConvexHull(const std::vector<Vec2<T>>& p) : m_points(p), m_sorted(false)
{

}
template <class T>
PointSetConvexHull<T>::PointSetConvexHull(std::vector<Vec2<T>>&& p) : m_points(p), m_sorted(false)
{

}
template <class T>
void PointSetConvexHull<T>::sortPoints() //made lhs.x <= rhs.x to omit warning about floating point comparsion
{
    std::sort(m_points.begin(), m_points.end(), [] (const Vec2<T>& lhs, const Vec2<T>& rhs) -> bool {return (lhs.x < rhs.x || (lhs.x <= rhs.x && lhs.y < rhs.y)); });
}
template <class T>
const Polygon<T>& PointSetConvexHull<T>::convexHull()
{
    return m_convexHull;
}
template <class T>
const std::vector<Vec2<T>>& PointSetConvexHull<T>::points()
{
    return m_points;
}
template <class T>
void PointSetConvexHull<T>::calculate()
{
    if(!m_sorted)
    {
        sortPoints();
        m_sorted = true;
    }
    int n = m_points.size(), k = 0;

    m_convexHull.vertices.clear();
    m_convexHull.vertices.resize(n * 2);

    // Build lower hull
    for(int i = 0; i < n; ++i)
    {
        while(k >= 2 && cross(m_convexHull.vertices[k - 2], m_convexHull.vertices[k - 1], m_points[i]) <= 0) --k;
        m_convexHull.vertices[k++] = m_points[i];
    }

    // Build upper hull
    for(int i = n - 2, t = k + 1; i >= 0; --i)
    {
        while(k >= t && cross(m_convexHull.vertices[k - 2], m_convexHull.vertices[k - 1], m_points[i]) <= 0) --k;
        m_convexHull.vertices[k++] = m_points[i];
    }

    m_convexHull.vertices.resize(k - 1);
}



template <class T>
T PointSetConvexHull<T>::cross(const Vec2<T>& o, const Vec2<T>& a, const Vec2<T>& b)
{
    return (a.x - o.x) * (b.y - o.y) - (a.y - o.y) * (b.x - o.x);
}
