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

    void calculate(const PointSetDelaunayTriangulation<T>& triangulation);
};

typedef UrquhartGraph<double> UrquhartGraphD;
typedef UrquhartGraph<float> UrquhartGraphF;
typedef UrquhartGraph<int> UrquhartGraphI;

#include "../src/UrquhartGraph.cpp"

#endif // URQUHARTGRAPH_H
