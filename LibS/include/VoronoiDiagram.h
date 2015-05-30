#ifndef VORONOIDIAGRAM_H
#define VORONOIDIAGRAM_H

template <class T>
class VoronoiDiagram
{
public:
    VoronoiDiagram(const std::vector<Vec2<T>>& points);
    VoronoiDiagram(std::vector<Vec2<T>>&& points);
    VoronoiDiagram(const PointSetDelaunayTriangulation<T>& triangulation);

    const std::vector<Polygon<T>>& polygons() const;
    const std::vector<Vec2<T>>& points() const;
    const std::set<typename Triangulation<T>::EdgeInd>& connections() const;
protected:
    std::vector<Polygon<T>> m_polygons;
    std::vector<Vec2<T>> m_points;
    std::set<typename Triangulation<T>::EdgeInd> m_connections; //TODO: do something about this EdgeInd class

    void calculate(const PointSetDelaunayTriangulation<T>& triangulation);

};

typedef VoronoiDiagram<double> VoronoiDiagramD;
typedef VoronoiDiagram<float> VoronoiDiagramF;
typedef VoronoiDiagram<int> VoronoiDiagramI;

#include "../src/VoronoiDiagram.cpp"

#endif // VORONOIDIAGRAM_H
