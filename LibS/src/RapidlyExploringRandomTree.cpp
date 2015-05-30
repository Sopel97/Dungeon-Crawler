template <class T, class NodeType>
RapidlyExploringRandomTree<T, NodeType>::RapidlyExploringRandomTree(const Shape2<T>& space) :
    m_space(space.clone()),
    m_randomEngine(std::make_unique<Random::Xorshift64Engine>()),
    m_randomPointPicker(*m_space)
{
    Vec2<T> center(space.center());
    m_nodes.push_back(new NodeType(center, nullptr));
}

template <class T, class NodeType>
RapidlyExploringRandomTree<T, NodeType>::RapidlyExploringRandomTree(const Shape2<T>& space, const Random::RandomEngineBase& randomEngine) :
    m_space(space.clone()),
    m_randomEngine(randomEngine.clone()),
    m_randomPointPicker(*m_space)
{
    Vec2<T> center(space.center());
    m_nodes.push_back(new NodeType(center));
}

template <class T, class NodeType>
const Shape2<T>& RapidlyExploringRandomTree<T, NodeType>::space() const
{
    return m_space;
}

template <class T, class NodeType>
template <class SomeShape>
void RapidlyExploringRandomTree<T, NodeType>::addObstacle(const SomeShape& someShape)
{
    m_obstacles.push_back(someShape.clone().release());
}

template <class T, class NodeType>
void RapidlyExploringRandomTree<T, NodeType>::generateNodes(size_t quantity)
{
    m_nodes.reserve(quantity + m_nodes.size());
    while(quantity--)
    {
        Vec2<T> sample;
        Edge closestEdge;
        do
        {
            sample = generateRandomSample();
            closestEdge = nearestEdge(sample);
        }
        while(!isWayClear(sample, closestEdge.targetVertex));

        NodeType* parentOfNewNode = nullptr;

        if(closestEdge.isNewNodeRequired)
        {
            NodeType* newNodeOnEdge = new NodeType(closestEdge.targetVertex, closestEdge.beginNode, closestEdge.endNode);
            closestEdge.beginNode->removeChild(closestEdge.endNode);
            closestEdge.beginNode->addChild(newNodeOnEdge);
            closestEdge.endNode->setParent(newNodeOnEdge);
            parentOfNewNode = newNodeOnEdge;
            addNode(newNodeOnEdge);
        }
        else
        {
            parentOfNewNode = closestEdge.closestEndNode;
        }

        NodeType* newNode = new NodeType(sample, parentOfNewNode); //closest node as parent
        parentOfNewNode->addChild(newNode);
        addNode(newNode);
    }
}
template <class T, class NodeType>
void RapidlyExploringRandomTree<T, NodeType>::addNode(NodeType* newNode)
{
    m_nodes.push_back(newNode);
}
template <class T, class NodeType>
const NodeType* RapidlyExploringRandomTree<T, NodeType>::nearestNode(const Vec2<T>& position) const
{
    const NodeType* closestNode = nullptr;
    T minDistance = std::numeric_limits<T>::max();
    if(!m_nodes.empty()) closestNode = m_nodes.back();
    for(const auto& node : m_nodes)
    {
        T distance = position.distanceTo(node->position());
        if(distance < minDistance)
        {
            minDistance = distance;
            closestNode = node;
        }
    }
    return closestNode;
}
template <class T, class NodeType>
NodeType* RapidlyExploringRandomTree<T, NodeType>::nearestNode(const Vec2<T>& position)
{
    NodeType* closestNode = nullptr;
    T minDistance = std::numeric_limits<T>::max();
    if(!m_nodes.empty()) closestNode = m_nodes.back();
    for(auto& node : m_nodes)
    {
        T distance = position.distanceTo(node->position());
        if(distance < minDistance)
        {
            minDistance = distance;
            closestNode = node;
        }
    }
    return closestNode;
}
template <class T, class NodeType>
typename RapidlyExploringRandomTree<T, NodeType>::Edge RapidlyExploringRandomTree<T, NodeType>::nearestEdge(const Vec2<T>& sample)
{
    Edge closestEdge {nullptr, nullptr, nullptr, Vec2<T>(), false};
    T closestEdgeDistance = std::numeric_limits<T>::max();
    for(auto& beginNode : m_nodes)
    {
        Vec2<T> beginNodePosition = beginNode->position();
        for(auto& endNode : beginNode->children())
        {
            Vec2<T> endNodePosition = endNode->position();

            Vec2<T> edge = endNodePosition - beginNodePosition;
            const T q = edge.quadrance();  // i.e. |w-v|^2 -  avoid a sqrt
            // Consider the line extending the segment, parameterized as v + t (w - v).
            // We find projection of point p onto the line.
            // It falls where t = [(p-v) . (w-v)] / |w-v|^2
            const T t = (sample - beginNodePosition).dot(edge) / q;

            Vec2<T> vertexOnEdge;
            NodeType* closestNode = nullptr;
            if(t <= 0.0)
            {
                vertexOnEdge = beginNodePosition;        // Beyond the 'v' end of the segment
                closestNode = beginNode;
            }
            else if(t >= 1.0)
            {
                vertexOnEdge = endNodePosition;   // Beyond the 'w' end of the segment
                closestNode = endNode;
            }
            else vertexOnEdge = beginNodePosition + edge * t;  // Projection falls on the segment
            T distance = sample.distanceTo(vertexOnEdge);

            if(distance < closestEdgeDistance)
            {
                closestEdge = Edge {beginNode, endNode, closestNode, vertexOnEdge, ((t > 0.0) && (t < 1.0))};
                closestEdgeDistance = distance;
            }
        }
    }
    if(closestEdge.beginNode == nullptr)
    {
        NodeType* onlyNode = m_nodes.back();
        Vec2<T> onlyNodePosition = onlyNode->position();
        closestEdge = Edge {onlyNode, onlyNode, onlyNode, onlyNodePosition, false};
    }
    return closestEdge;
}
template <class T, class NodeType>
Vec2<T> RapidlyExploringRandomTree<T, NodeType>::generateRandomSample() const
{
    return m_randomPointPicker.pickRandomPoint(*m_randomEngine);
}

template <class T, class NodeType>
bool RapidlyExploringRandomTree<T, NodeType>::isWayClear(const Vec2<T>& begin, const Vec2<T>& end) const
{
    LineSegment<T> edge(begin, end);
    if(!isWayInsideShape(begin, end)) return false;
    for(const auto& obstacle : m_obstacles)
    {
        if(obstacle->intersects(edge))
        {
            return false;
        }
    }
    return true;
}

template <class T, class NodeType>
bool RapidlyExploringRandomTree<T, NodeType>::isWayInsideShape(const Vec2<T>& begin, const Vec2<T>& end) const //TODO: make use of it
{
    return m_space->contains(LineSegment<T>(begin, end)); //this will be enabled when the containment methods get implemented
    return true;
}
template <class T, class NodeType>
std::vector<LineSegment<T>> RapidlyExploringRandomTree<T, NodeType>::edges() const
{
    std::vector<LineSegment<T>> allEdges;
    allEdges.reserve(m_nodes.size() - 1);

    for(const auto& node : m_nodes)
    {
        Vec2<T> position = node->position();
        for(const auto& child : node->children())
        {
            allEdges.emplace_back(position, child->position());
        }
    }
    return allEdges;
}
template <class T, class NodeType>
RapidlyExploringRandomTree<T, NodeType>::~RapidlyExploringRandomTree()
{
    for(auto& obstacle : m_obstacles)
    {
        delete obstacle;
    }
    for(auto& node : m_nodes)
    {
        delete node;
    }
}
