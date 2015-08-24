template <class T>
VoronoiDiagram<T>::VoronoiDiagram(const std::vector<Vec2<T>>& points)
{
    calculate(PointSetDelaunayTriangulation<T>(points));
}
template <class T>
VoronoiDiagram<T>::VoronoiDiagram(std::vector<Vec2<T>>&& points)
{
    calculate(PointSetDelaunayTriangulation<T>(std::move(points)));
}
template <class T>
VoronoiDiagram<T>::VoronoiDiagram(const PointSetDelaunayTriangulation<T>& triangulation)
{
    calculate(triangulation);
}

template <class T>
void VoronoiDiagram<T>::calculate(const PointSetDelaunayTriangulation<T>& triangulation)
{
    class AngleSort
    {
        // From http://stackoverflow.com/questions/7774241/sort-points-by-angle-from-given-axis
        // by @Ben Voigt

    public:
        AngleSort(const Vec2<T>& origin, const Vec2<T>& reference) : m_origin(origin), m_dreference(reference - origin) {}
        bool operator()(const Vec2<T>& a, const Vec2<T>& b) const
        {
            const Vec2<T> da = a - m_origin;
            const Vec2<T> db = b - m_origin;


            const double detb = m_dreference.cross(db);

            // nothing is less than zero degrees
            if(std::abs(detb) <= EPSILON && db.dot(m_dreference) >= T(0)) return false;

            const double deta = m_dreference.cross(da);

            // zero degrees is less than anything else
            if(std::abs(deta) <= EPSILON && da.dot(m_dreference) >= T(0)) return true;

            if(deta * detb >= T(0))
            {
                // both on same side of reference, compare to each other
                return da.cross(db) > T(0);
            }

            // TODO: polygons for which only one side of reference is reached should be removed because
            //       these are the one that stretch to infinity and should be replaced by rays
            //       Somehow make it give such information in return

            // vectors "less than" zero degrees are actually large, near 2 pi
            return deta > T(0);


            //return std::atan2(da.y, da.x) < std::atan2(db.y, db.x);
        }
    protected:
        Vec2<T> m_origin;
        Vec2<T> m_dreference;
    };

    m_points.clear();
    m_polygons.clear();
    m_connections.clear();

    const auto& triangles = triangulation.closedTriangles();
    const auto& delaunayPoints = triangulation.points();

    std::vector<std::vector<Vec2<T>>> polygonsVertices(delaunayPoints.size()); // vertices of polygon around vertex i (for i being index of every point in m_points)
    std::vector<std::vector<size_t>> polygonsIndices(delaunayPoints.size()); // indices of vertices of polygon around vertex i (for i being index of every point in m_points)
    // for vertices on the boundary it will form only part of the polygon.
    // These need to be recognized and either extended to infinity or removed and replaced by two rays (the second one is on todo list)

    m_points.reserve(triangles.size());

    size_t polyIndex = 0;
    for(const auto& triangle : triangles)
    {
        const Vec2<T>& currentCircumcircle = triangle.circumcircle.origin;

        m_points.push_back(currentCircumcircle);

        polygonsVertices[triangle.i].push_back(currentCircumcircle);
        polygonsVertices[triangle.j].push_back(currentCircumcircle);
        polygonsVertices[triangle.k].push_back(currentCircumcircle);

        polygonsIndices[triangle.i].push_back(polyIndex);
        polygonsIndices[triangle.j].push_back(polyIndex);
        polygonsIndices[triangle.k].push_back(polyIndex);

        ++polyIndex;
    }

    size_t i = 0u;
    for(auto& polygonUnorderedPoints : polygonsVertices)
    {
        std::sort(polygonUnorderedPoints.begin(), polygonUnorderedPoints.end(), AngleSort(delaunayPoints[i++], polygonUnorderedPoints.front()));
    }

    // TODO: some of the polygons are still to be removed
    //       do rays where polygons would stretch to infinity

    m_polygons.reserve(polygonsVertices.size());
    for(auto& polygon : polygonsVertices)
    {
        m_polygons.emplace_back(std::move(polygon));
    }
    for(auto& polygon : polygonsIndices)
    {
        size_t size = polygon.size();
        for(size_t i = 0; i < size; ++i)
        {
            m_connections.insert(typename Triangulation<T>::EdgeInd(polygon[i], polygon[(i + 1) % size]));
        }
    }

}
template <class T>
const std::vector<Polygon<T>>& VoronoiDiagram<T>::polygons() const
{
    return m_polygons;
}
template <class T>
const std::vector<Vec2<T>>& VoronoiDiagram<T>::points() const
{
    return m_points;
}
template <class T>
const std::set<typename Triangulation<T>::EdgeInd>& VoronoiDiagram<T>::connections() const
{
    return m_connections;
}
