#ifndef URQUHARTGRAPH_H
#define URQUHARTGRAPH_H

template <class T, class NodeType>
class UrquhartGraph
{
public:
    UrquhartGraph(const std::vector<Vec2<T>>& points);
    UrquhartGraph(std::vector<Vec2<T>>&& points);
    UrquhartGraph(const PointSetDelaunayTriangulation<T>& triangulation);

protected:
    std::vector<NodeType*> m_nodes;

    void fromTriangulation(const PointSetDelaunayTriangulation<T>& triangulation);
    void fromChosenTriangulationConnections(const std::vector<Vec2<T>>& points, const std::set<typename Triangulation<T>::EdgeInd>& connections);
};

typedef UrquhartGraph<double> UrquhartGraphD;
typedef UrquhartGraph<float> UrquhartGraphF;
typedef UrquhartGraph<int> UrquhartGraphI;

extern template class UrquhartGraph<double>;
extern template class UrquhartGraph<float>;
extern template class UrquhartGraph<int>;

#include "../src/UrquhartGraph.cpp"

#endif // URQUHARTGRAPH_H
