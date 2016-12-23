#pragma once

#include "../Fwd.h"
#include "Vec2.h"
#include "Vec3.h"
#include "Vec4.h"
#include "../Util.h"
#include "GraphRepresentationTags.h"

#include <vector>
#include <set>

//TODO consider making methods that return iterators, because now finding edge does the same as checking for existence of edge.
// to check for existence and then request edge would be waste of resources
namespace ls
{
    namespace detail
    {
        template <class T>
        struct HasMethod_position
        {
        private:
            using yes = std::true_type;
            using no = std::false_type;

            template<typename U> static auto test(int) -> decltype(std::declval<U>().position(), yes());

            template<typename> static no test(...);

        public:

            static constexpr bool value = std::is_same<decltype(test<T>(0)), yes>::value;
        };

        template <class T>
        struct HasMethod_weight
        {
        private:
            using yes = std::true_type;
            using no = std::false_type;

            template<typename U> static auto test(int) -> decltype(std::declval<U>().weight(), yes());

            template<typename> static no test(...);

        public:

            static constexpr bool value = std::is_same<decltype(test<T>(0)), yes>::value;
        };

        template <class NodeT, bool hasMethodPosition = HasMethod_position<NodeT>::value>
        auto distance(const NodeT& v1, const NodeT& v2)
            -> typename std::enable_if<hasMethodPosition, decltype(v2.position().distance(v1.position()))>::type
        {
            return v2.position().distance(v1.position());
        }

        template <class NodeT, bool hasMethodPosition = HasMethod_position<NodeT>::value>
        auto distance(const NodeT&, const NodeT&)
            -> typename std::enable_if<!hasMethodPosition, int>::type
        {
            return 1;
        }

        template <class EdgeT, bool hasMethodWeight = HasMethod_weight<EdgeT>::value>
        auto weight(const EdgeT& edge)
            -> typename std::enable_if<hasMethodWeight, decltype(edge.weight())>::type
        {
            return edge.weight();
        }

        template <class EdgeT, bool hasMethodWeight = HasMethod_weight<EdgeT>::value>
        auto weight(const EdgeT&)
            -> typename std::enable_if<!hasMethodWeight, int>::type
        {
            return 1;
        }
    }

    namespace
    {
        class GraphNode
        {

        };

        class GraphHalfEdge
        {
        public:
            int v;

            GraphHalfEdge(int _v);
        };

        bool operator==(const GraphHalfEdge& lhs, int rhs)
        {
            return lhs.v == rhs;
        }
        bool operator==(int lhs, const GraphHalfEdge& rhs)
        {
            return lhs == rhs.v;
        }
        bool operator<(const GraphHalfEdge& lhs, int rhs)
        {
            return lhs.v < rhs;
        }
        bool operator<(int lhs, const GraphHalfEdge& rhs)
        {
            return lhs < rhs.v;
        }

        bool operator==(const GraphHalfEdge& lhs, const GraphHalfEdge& rhs)
        {
            return lhs.v == rhs.v;
        }
        bool operator<(const GraphHalfEdge& lhs, const GraphHalfEdge& rhs)
        {
            return lhs.v < rhs.v;
        }

        class GraphEdge
        {
        public:
            int u, v;

            GraphEdge(int _u, int _v);
            GraphEdge(int _u, const GraphHalfEdge& halfEdge);
        };

        bool operator==(const GraphEdge& lhs, const GraphEdge& rhs)
        {
            return std::tie(lhs.u, lhs.v) == std::tie(rhs.u, rhs.v);
        }
        bool operator<(const GraphEdge& lhs, const GraphEdge& rhs)
        {
            return std::tie(lhs.u, lhs.v) < std::tie(rhs.u, rhs.v);
        }
    }


    template <class T, int Dim>
    class EuclideanGraphNode : public GraphNode
    {
    public:
        using ValueType = T;
        using VectorType = typename Util::VectorType<T, Dim>::type;
        constexpr static int dimensionality = Dim;

    private:
        VectorType m_position;
    public:
        EuclideanGraphNode(const VectorType& position);

        const VectorType& position() const;
    };

    using EuclideanGraphNode2D = EuclideanGraphNode<double, 2>;
    using EuclideanGraphNode2F = EuclideanGraphNode<float, 2>;

    extern template class EuclideanGraphNode<double, 2>;
    extern template class EuclideanGraphNode<float, 2>;

    template <class T>
    class WeightedGraphHalfEdge : public GraphHalfEdge
    {
    private:
        T m_weight;
    public:

        WeightedGraphHalfEdge(int _v, T w = T(1));

        T weight() const;
    };

    template <class T>
    class WeightedGraphEdge : public GraphEdge
    {
    private:
        T m_weight;
    public:

        WeightedGraphEdge(int _u, int _v, T w = T(1));
        WeightedGraphEdge(int _u, const WeightedGraphHalfEdge<T>& halfEdge);

        T weight() const;
    };

    using WeightedGraphEdgeD = WeightedGraphEdge<double>;
    using WeightedGraphEdgeF = WeightedGraphEdge<float>;

    extern template class WeightedGraphEdge<double>;
    extern template class WeightedGraphEdge<float>;


    //TODO: enforce and specify the following better
    template <int Dim>
    class DefaultEuclideanGraphStorageTypeReference
    {
    public:

        template <class T>
        using NodeType = EuclideanGraphNode<T, Dim>;

        template <class T>
        using EdgeType = GraphEdge;
        template <class T>
        using HalfEdgeType = GraphHalfEdge;

        /*
        std::vector behaviour
        */
        template <class T>
        using NodeArrayStorageType = std::vector<T>;

        /*
            std::set behaviour
        */
        template <class T>
        using NeighbourSetStorageType = std::set<T>;


        /*
            Requires that following are defined for Container:
            - Container(width, height, value)
            - operator(x, y)
            - width()

        */
        template <class T>
        using AdjacencyMatrixStorageType = Array2<T>;


        /*
            std::set behaviour
        */
        template <class T>
        using EdgeSetStorageType = std::set<T>;

        /*
            std::vector behaviour
        */
        template <class T>
        using EdgeArrayStorageType = std::vector<T>;
    };

    //NO duplicate edges allowed
    template <class T, class GraphRepresentationTag, bool IsDirected, class StorageTypeReference>
    class Graph
    {
        Graph() = delete;
    };

    template <class T, bool IsDirected, class StorageTypeReference>
    class Graph<T, NeighbourEdgeSetWithEdgeListTag, IsDirected, StorageTypeReference>
    {
    public:

        using ValueType = T;
        using NodeType = typename StorageTypeReference::template NodeType<T>;
        using EdgeType = typename StorageTypeReference::template EdgeType<T>;
        using HalfEdgeType = typename StorageTypeReference::template HalfEdgeType<T>;
        using VectorType = typename NodeType::VectorType;
        using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
        using NeighbourEdgeSetStorageType = typename StorageTypeReference::template NeighbourSetStorageType<int>;
        using NeighbourEdgeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NeighbourEdgeSetStorageType>;
        using EdgeArrayStorageType = typename StorageTypeReference::template EdgeArrayStorageType<EdgeType>;
        using Representation = NeighbourEdgeSetWithEdgeListTag;
        static constexpr bool isDirected = IsDirected;
        static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
        static constexpr bool isWeighted = detail::HasMethod_weight<EdgeType>::value && detail::HasMethod_weight<HalfEdgeType>::value;

        static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");
        static_assert(std::is_base_of<GraphEdge, EdgeType>::value, "EdgeType must be derived from GraphEdge");
        static_assert(std::is_base_of<GraphHalfEdge, HalfEdgeType>::value, "HalfEdgeType must be derived from GraphHalfEdge");

        Graph() = default;

        template <class... Args>
        int emplaceNode(Args&&... args);
        template <class... Args>
        void emplaceEdge(int u, int v, Args&&... args);

        void removeNode(int v);
        void removeEdge(int u, int v);

        const NodeArrayStorageType& nodes() const &;
        NodeArrayStorageType nodes() && ;
        const EdgeArrayStorageType& edges() const &;
        EdgeArrayStorageType edges() && ;

        const NodeType& node(int v) const;
        const EdgeType& edge(int u, int v) const;
        const NeighbourEdgeSetStorageType& edgesToNeighbours(int v) const;
        bool existsEdge(int u, int v) const;
        int numberOfNodes() const;

        int nodeDegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeIndegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeOutdegree(int v) const;

        template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
        Ret edgeVector(int u, int v) const;

        ValueType edgeWeight(int u, int v) const;
        ValueType edgeWeight(const EdgeType& edge) const;

    protected:
        NodeArrayStorageType m_nodes;
        NeighbourEdgeArrayStorageType m_edgesToNeighbours; //holds indices of edges, NOT neighbour nodes
        EdgeArrayStorageType m_edges;
    };

    template <class T, bool IsDirected, class StorageTypeReference>
    class Graph<T, NeighbourSetTag, IsDirected, StorageTypeReference>
    {
    public:
        using ValueType = T;
        using NodeType = typename StorageTypeReference::template NodeType<T>;
        using EdgeType = typename StorageTypeReference::template EdgeType<T>;
        using HalfEdgeType = typename StorageTypeReference::template HalfEdgeType<T>;
        using VectorType = typename NodeType::VectorType;
        using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
        using NeighbourSetStorageType = typename StorageTypeReference::template NeighbourSetStorageType<HalfEdgeType>;
        using NeighbourArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NeighbourSetStorageType>;
        using Representation = NeighbourSetTag;
        static constexpr bool isDirected = IsDirected;
        static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
        static constexpr bool isWeighted = detail::HasMethod_weight<HalfEdgeType>::value;

        static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");
        static_assert(std::is_base_of<GraphEdge, EdgeType>::value, "EdgeType must be derived from GraphEdge");
        static_assert(std::is_base_of<GraphHalfEdge, HalfEdgeType>::value, "HalfEdgeType must be derived from GraphHalfEdge");

        Graph() = default;

        template <class... Args>
        int emplaceNode(Args&&... args);
        template <class... Args>
        void emplaceEdge(int u, int v, Args&&... args);

        void removeNode(int v);
        void removeEdge(int u, int v);

        const NodeArrayStorageType& nodes() const &;
        NodeArrayStorageType nodes() && ;
        const NodeType& node(int v) const;
        EdgeType edge(int u, int v) const;
        const NeighbourSetStorageType& neighbours(int v) const;
        bool existsEdge(int u, int v) const;
        int numberOfNodes() const;

        int nodeDegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeIndegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeOutdegree(int v) const;

        template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
        Ret edgeVector(int u, int v) const;

        ValueType edgeWeight(int u, int v) const;
        ValueType edgeWeight(const EdgeType& edge) const;

    protected:
        NodeArrayStorageType m_nodes;
        NeighbourArrayStorageType m_neighbours;
    };

    template <class T, bool IsDirected, class StorageTypeReference>
    class Graph<T, AdjacencyMatrixTag, IsDirected, StorageTypeReference>
    {
    public:
        using ValueType = T;
        using NodeType = typename StorageTypeReference::template NodeType<T>;
        using VectorType = typename NodeType::VectorType;
        using Representation = AdjacencyMatrixTag;
        using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
        using AdjacencyMatrixStorageType = typename StorageTypeReference::template AdjacencyMatrixStorageType<bool>;
        using NeighbourSetStorageType = typename StorageTypeReference::template NeighbourSetStorageType<int>;
        static constexpr bool isDirected = IsDirected;
        static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
        static constexpr bool isWeighted = false;

        static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");

        Graph() = default;

        template <class... Args>
        int emplaceNode(Args&&... args);
        void emplaceEdge(int u, int v);

        void removeNode(int u);
        void removeEdge(int u, int v);

        const NodeArrayStorageType& nodes() const &;
        NodeArrayStorageType nodes() && ;
        const NodeType& node(int v) const;
        NeighbourSetStorageType neighbours(int v) const;
        bool existsEdge(int u, int v) const;
        int numberOfNodes() const;

        int nodeDegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeIndegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeOutdegree(int v) const;

        template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
        Ret edgeVector(int u, int v) const;

    protected:
        NodeArrayStorageType m_nodes;
        AdjacencyMatrixStorageType m_adjacencyMatrix; //[x][y] is set to true if edge x,y exists

        void resizeAdjacencyMatrix(int newSize);

    };

    template <class T, bool IsDirected, class StorageTypeReference>
    class Graph<T, EdgeSetTag, IsDirected, StorageTypeReference>
    {
    public:
        using ValueType = T;
        using NodeType = typename StorageTypeReference::template NodeType<T>;
        using EdgeType = typename StorageTypeReference::template EdgeType<T>;
        using VectorType = typename NodeType::VectorType;
        using Representation = EdgeSetTag;
        using NodeArrayStorageType = typename StorageTypeReference::template NodeArrayStorageType<NodeType>;
        using EdgeSetStorageType = typename StorageTypeReference::template EdgeSetStorageType<EdgeType>;
        static constexpr bool isDirected = IsDirected;
        static constexpr bool isEuclidean = detail::HasMethod_position<NodeType>::value;
        static constexpr bool isWeighted = detail::HasMethod_weight<EdgeType>::value;

        static_assert(std::is_base_of<GraphNode, NodeType>::value, "NodeType must be derived from GraphNode");
        static_assert(std::is_base_of<GraphEdge, EdgeType>::value, "EdgeType must be derived from GraphEdge");

        Graph() = default;

        template <class... Args>
        int emplaceNode(Args&&... args);
        template <class... Args>
        void emplaceEdge(int u, int v, Args&&... args);

        void removeNode(int v);
        void removeEdge(int u, int v);

        const NodeArrayStorageType& nodes() const &;
        NodeArrayStorageType nodes() && ;
        const NodeType& node(int v) const;
        const EdgeType& edge(int u, int v) const;
        const EdgeSetStorageType& edges() const &;
        EdgeSetStorageType edges() && ;
        bool existsEdge(int u, int v) const;
        int numberOfNodes() const;

        int nodeDegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeIndegree(int v) const;
        template <bool D = IsDirected, class SFINAE = std::enable_if_t<D == true, void>>
        int nodeOutdegree(int v) const;

        template <bool isEuclidean_ = isEuclidean, class Ret = typename std::enable_if<isEuclidean_, VectorType>::type>
        Ret edgeVector(int u, int v) const;

        ValueType edgeWeight(int u, int v) const;
        ValueType edgeWeight(const EdgeType& edge) const;

    protected:
        NodeArrayStorageType m_nodes;
        EdgeSetStorageType m_edges;
    };


    using AdjacencyListEuclideanDigraph2D = Graph<double, NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    using AdjacencyListEuclideanDiGraph2F = Graph<float, NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    using AdjacencyMatrixEuclideanDiGraph2D = Graph<double, AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    using AdjacencyMatrixEuclideanDiGraph2F = Graph<float, AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    using EdgeListEuclideanDiGraph2D = Graph<double, EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    using EdgeListEuclideanDiGraph2F = Graph<float, EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    using AdjacencyListEuclideanGraph2D = Graph<double, NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    using AdjacencyListEuclideanGraph2F = Graph<float, NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    using AdjacencyMatrixEuclideanGraph2D = Graph<double, AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    using AdjacencyMatrixEuclideanGraph2F = Graph<float, AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    using EdgeListEuclideanGraph2D = Graph<double, EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    using EdgeListEuclideanGraph2F = Graph<float, EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;



    extern template class Graph<double, NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, NeighbourSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    extern template class Graph<double, AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, AdjacencyMatrixTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    extern template class Graph<double, EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, EdgeSetTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    extern template class Graph<double, NeighbourEdgeSetWithEdgeListTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, NeighbourEdgeSetWithEdgeListTag, true, DefaultEuclideanGraphStorageTypeReference<2>>;

    extern template class Graph<double, NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, NeighbourSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    extern template class Graph<double, AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, AdjacencyMatrixTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    extern template class Graph<double, EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, EdgeSetTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;

    extern template class Graph<double, NeighbourEdgeSetWithEdgeListTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
    extern template class Graph<float, NeighbourEdgeSetWithEdgeListTag, false, DefaultEuclideanGraphStorageTypeReference<2>>;
}

#include "../src/Graph.cpp"
