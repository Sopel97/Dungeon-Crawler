#ifndef RAPIDLYEXPLORINGRANDOMTREE_H
#define RAPIDLYEXPLORINGRANDOMTREE_H

template <class T, class NodeType> //NodeType* must be compatibile with TreeNode
class RapidlyExploringRandomTree
{
public:

    RapidlyExploringRandomTree(const Shape2<T>& space);
    RapidlyExploringRandomTree(const Shape2<T>& space, const Random::RandomEngineBase& randomEngine);

    ~RapidlyExploringRandomTree();

    template <class SomeShape>
    void addObstacle(const SomeShape& someShape);

    void generateNodes(size_t quantity);

    void addNode(NodeType* newNode);

    const NodeType* nearestNode(const Vec2<T>& position) const;
    NodeType* nearestNode(const Vec2<T>& position);

    Vec2<T> generateRandomSample() const;

    bool isWayClear(const Vec2<T>& begin, const Vec2<T>& end) const;
    bool isWayInsideShape(const Vec2<T>& begin, const Vec2<T>& end) const;

    std::vector<LineSegment<T>> edges() const;

    const Shape2<T>& space() const;

protected:
    struct Edge
    {
        NodeType* beginNode;
        NodeType* endNode;
        NodeType* closestEndNode;
        Vec2<T> targetVertex;
        bool isNewNodeRequired;
    };

    std::unique_ptr<Shape2<T>> m_space;
    std::unique_ptr<Random::RandomEngineBase> m_randomEngine;
    std::vector<Shape2<T>*> m_obstacles; //owning
    std::vector<NodeType*> m_nodes; //owning
    RandomShapePicker<T> m_randomPointPicker;

    Edge nearestEdge(const Vec2<T>& sample);
};

typedef RapidlyExploringRandomTree<double> RapidlyExploringRandomTreeD;
typedef RapidlyExploringRandomTree<float> RapidlyExploringRandomTreeF;
typedef RapidlyExploringRandomTree<int> RapidlyExploringRandomTreeI;

#include "../src/RapidlyExploringRandomTree.cpp"

#endif // RAPIDLYEXPLORINGRANDOMTREE_H
