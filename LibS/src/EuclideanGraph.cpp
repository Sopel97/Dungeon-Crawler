template <class T, class NodeType>
EuclideanGraph<T, NodeType>::EuclideanGraph()
{

}

template <class T, class NodeType>
EuclideanGraph<T, NodeType>::~EuclideanGraph()
{
    for(auto& node : m_nodes) delete node;
}

template <class T, class NodeType>
void EuclideanGraph<T, NodeType>::fromTriangulation(const Triangulation<T>& triangulation)
{
    fromChosenTriangulationConnections(triangulation.points(), triangulation.connections());
}

template <class T, class NodeType>
void EuclideanGraph<T, NodeType>::fromChosenTriangulationConnections(const std::vector<Vec2<T>>& points, const std::set<typename Triangulation<T>::EdgeInd>& connections)
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
