template <class T, class NodeType>
UrquhartGraph<T, NodeType>::UrquhartGraph(const std::vector<Vec2<T>>& points)
{
    fromTriangulation(PointSetDelaunayTriangulation<T>(points));
}
template <class T, class NodeType>
UrquhartGraph<T, NodeType>::UrquhartGraph(std::vector<Vec2<T>>&& points)
{
    fromTriangulation(PointSetDelaunayTriangulation<T>(std::move(points)));
}
template <class T, class NodeType>
UrquhartGraph<T, NodeType>::UrquhartGraph(const PointSetDelaunayTriangulation<T>& triangulation)
{
    fromTriangulation(triangulation);
}

template <class T, class NodeType>
void UrquhartGraph<T, NodeType>::fromTriangulation(const PointSetDelaunayTriangulation<T>& triangulation)
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

    fromChosenTriangulationConnections(points, connections);
}
template <class T, class NodeType>
void UrquhartGraph<T, NodeType>::fromChosenTriangulationConnections(const std::vector<Vec2<T>>& points, const std::set<typename Triangulation<T>::EdgeInd>& connections)
{
    for(auto& node : m_nodes) delete node;
    m_nodes.clear();

    m_nodes.reserve(points.size());
    for(const auto& point : points)
    {
        m_nodes.push_back(new NodeType(point));
    }

    for(const auto& connection : connections)
    {
        auto& node1 = m_nodes[connection.i];
        auto& node2 = m_nodes[connection.j];

        node1->addChild(node2);
        node2->addChild(node1);
    }
}

