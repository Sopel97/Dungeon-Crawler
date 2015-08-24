#ifndef POINTSETDELAUNAYTRIANGULATION_H
#define POINTSETDELAUNAYTRIANGULATION_H

/*
    Most of the code is from
    https://github.com/ironwallaby/delaunay/blob/master/delaunay.js
*/

//TODO: Change name to DelaunayTriangulation (when triangulation of polygons is added it will take either vector of points or polygon, it will be properly dispatched trough constructors)
template <class T>
class PointSetDelaunayTriangulation : public Triangulation<T>
{
public:
    struct CircumCircle
    {
        size_t i; //indices of the vertices of the triangle
        size_t j;
        size_t k;
        Circle<T> circumcircle; //during computation stored radius will be squared to avoid unnecessary sqrt
    };

    PointSetDelaunayTriangulation(const std::vector<Vec2<T>>& points);
    PointSetDelaunayTriangulation(std::vector<Vec2<T>>&& points);

    virtual ~PointSetDelaunayTriangulation(){}


    virtual size_t numberOfPoints() const;
    virtual const std::vector<Vec2<T>>& points() const;
    virtual const Vec2<T>& point(size_t i) const;

    const std::vector<CircumCircle>& closedTriangles() const;

protected:
    std::vector<Vec2<T>> m_points;
    std::vector<CircumCircle> m_closedTriangles; //stores triangles as 3 indices i,j,k and their circumcircles

    void calculate();
    Triangle<T> superTriangle(const std::vector<Vec2<T>>& vertices) const;
    CircumCircle circumcircle(const std::vector<Vec2<T>>& vertices, size_t i, size_t j, size_t k) const;
};

typedef PointSetDelaunayTriangulation<double> PointSetDelaunayTriangulationD;
typedef PointSetDelaunayTriangulation<float> PointSetDelaunayTriangulationF;
typedef PointSetDelaunayTriangulation<int> PointSetDelaunayTriangulationI;

extern template class PointSetDelaunayTriangulation<double>;
extern template class PointSetDelaunayTriangulation<float>;
extern template class PointSetDelaunayTriangulation<int>;

#include "../src/PointSetDelaunayTriangulation.cpp"

#endif // POINTSETDELAUNAYTRIANGULATION_H
