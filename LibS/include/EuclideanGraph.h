#ifndef EUCLIDEANGRAPH_H
#define EUCLIDEANGRAPH_H

template <class T, class NodeType>
class EuclideanGraph
{
    public:
        EuclideanGraph();

        virtual ~EuclideanGraph();
    protected:
        std::vector<NodeType*> m_nodes;

        void fromTriangulation(const Triangulation<T>& triangulation);
        void fromChosenTriangulationConnections(const std::vector<Vec2<T>>& points, const std::set<typename Triangulation<T>::EdgeInd>& connections);
};

typedef EuclideanGraph<double> EuclideanGraphD;
typedef EuclideanGraph<float> EuclideanGraphF;
typedef EuclideanGraph<int> EuclideanGraphI;

extern template class EuclideanGraph<double>;
extern template class EuclideanGraph<float>;
extern template class EuclideanGraph<int>;

#include "../src/EuclideanGraph.cpp"

#endif // EUCLIDEANGRAPH_H
