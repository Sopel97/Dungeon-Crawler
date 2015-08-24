#ifndef GRAPH_H_INCLUDED
#define GRAPH_H_INCLUDED

#include "Shapes.h"
#include "Nodes.h"
#include "Triangulation.h"

namespace ls
{
    template <class T, class NodeType = GraphNode<T>> class EuclideanGraph;
    template <class T, class NodeType = GraphNode<T>> class UrquhartGraph;


#include "include/EuclideanGraph.h"
#include "include/UrquhartGraph.h"
}

#endif // GRAPH_H_INCLUDED
