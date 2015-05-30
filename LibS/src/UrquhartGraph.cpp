template <class T, class NodeType>
UrquhartGraph<T, NodeType>::UrquhartGraph(const std::vector<Vec2<T>>& points)
{
    calculate(PointSetDelaunayTriangulation<T>(points));
}
template <class T, class NodeType>
UrquhartGraph<T, NodeType>::UrquhartGraph(std::vector<Vec2<T>>&& points)
{
    calculate(PointSetDelaunayTriangulation<T>(std::move(points)));
}
template <class T, class NodeType>
UrquhartGraph<T, NodeType>::UrquhartGraph(const PointSetDelaunayTriangulation<T>& triangulation)
{
    calculate(triangulation);
}

template <class T, class NodeType>
void UrquhartGraph<T, NodeType>::calculate(const PointSetDelaunayTriangulation<T>& triangulation)
{
    std::set<typename Triangulation<T>::EdgeInd> connections;
    const auto& points = triangulation.points();
    for(const auto& triangle : triangulation.triangles())
    {
        auto isMaxOf3 = [](T& a, T& b, T& c){return a >= b && a >= c;};
        T lij = points[triangle.i].distanceTo(points[triangle.j]);
        T ljk = points[triangle.j].distanceTo(points[triangle.k]);
        T lki = points[triangle.k].distanceTo(points[triangle.i]);
        if(isMaxOf3(lij, ljk, lki)) //ij is longest
        {
            connections.insert({typename Triangulation<T>::EdgeInd(triangle.j, triangle.k),
                                typename Triangulation<T>::EdgeInd(triangle.k, triangle.i)});
        }
        else if(isMaxOf3(ljk, lki, lij)) //jk is longest
        {
            connections.insert({typename Triangulation<T>::EdgeInd(triangle.k, triangle.i),
                                typename Triangulation<T>::EdgeInd(triangle.i, triangle.j)});
        }
        else //ki is longest
        {
            connections.insert({typename Triangulation<T>::EdgeInd(triangle.i, triangle.j),
                                typename Triangulation<T>::EdgeInd(triangle.j, triangle.k)});
        }
    }

    this->fromChosenTriangulationConnections(points, connections);
}
