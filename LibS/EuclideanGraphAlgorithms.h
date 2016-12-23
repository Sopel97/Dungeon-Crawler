#pragma once

#include <vector>
#include <list>
#include <stack>
#include <algorithm>

#include "Graph.h"

namespace ls
{
    //TODO: consider using unordered set for storing visited edges, right now it is O(n^2) due to searching for reversed edge
    template <class T, class StorageTypeReference>
    GraphPath closedEulerianPath(const Graph<T, NeighbourSetTag, false, StorageTypeReference>& graph)
    {
        using GraphType = Graph<T, NeighbourSetTag, false, StorageTypeReference>;
        using NeighbourSetStorageType = typename GraphType::NeighbourSetStorageType;

        using std::find;
        using std::begin;
        using std::end;

        struct RecursionLevel
        {
            int currentVertex;
            std::list<int>::iterator onePastCurrentIterator;
        };

        const int numberOfVertices = graph.numberOfNodes();

        std::vector<int> flattenedAdjacencyListsOffsets;
        flattenedAdjacencyListsOffsets.reserve(numberOfVertices);
        int numberOfEdges = 0;
        for(int v = 0; v < numberOfVertices; ++v)
        {
            flattenedAdjacencyListsOffsets.emplace_back(numberOfEdges);
            const auto& neighbours = graph.neighbours(v);

            numberOfEdges += neighbours.size();
        }

        std::vector<uint8_t> visitedEdges(numberOfEdges, false); //laid as flattened adjacency lists
        std::vector<typename NeighbourSetStorageType::const_iterator> nextNeighbours;
        std::vector<int> nextNeighboursIndices(numberOfVertices, 0);
        nextNeighbours.reserve(numberOfVertices);
        for (int i = 0; i < numberOfVertices; ++i)
        {
            nextNeighbours.push_back(begin(graph.neighbours(i)));
        }

        ///////////////////////////
        
        std::list<int> path;
        std::vector<RecursionLevel> callStackContainer;
        callStackContainer.reserve(numberOfEdges + 1);
        std::stack<RecursionLevel, std::vector<RecursionLevel>> callStack(std::move(callStackContainer));

        int currentVertex = 0;
        path.emplace_back(currentVertex);
        std::list<int>::iterator onePastCurrentIterator = path.end();

        do
        {
            int neighbourIndex = nextNeighboursIndices[currentVertex];
            auto neighbourIter = nextNeighbours[currentVertex];
            auto endIter = end(graph.neighbours(currentVertex));
            for(; neighbourIter != endIter; ++neighbourIter, ++neighbourIndex)
            {
                int neighbourVertex = (*neighbourIter).v;
                const auto& neighboursOfNeighbour = graph.neighbours(neighbourVertex);

                //we only mark edges with u < v instead of both
                uint8_t* isVisited;
                if(currentVertex < neighbourVertex)
                {
                    isVisited = &(visitedEdges[flattenedAdjacencyListsOffsets[currentVertex] + neighbourIndex]);
                }
                else
                {
                    int j = 0;
                    //this seems to be unavoidable, unless storage method changes
                    for (auto& e : neighboursOfNeighbour)
                    {
                        if (e.v == currentVertex) break;
                        ++j;
                    }

                    isVisited = &(visitedEdges[flattenedAdjacencyListsOffsets[neighbourVertex] + j]); //the same edge, but from the other side
                }

                if(*isVisited == false)
                {
                    //move to the next vertex and remember state of this iteration if there is any neighbours left to check
                    auto elementIter = path.emplace(onePastCurrentIterator, neighbourVertex);
                    callStack.push({ currentVertex, elementIter });
                    nextNeighbours[currentVertex] = std::next(neighbourIter, 1);
                    nextNeighboursIndices[currentVertex] = neighbourIndex + 1;

                    currentVertex = neighbourVertex;
                    neighbourIter = nextNeighbours[currentVertex];
                    neighbourIndex = nextNeighboursIndices[currentVertex];

                    *isVisited = true;
                    break;
                }
            }
            if(neighbourIter == end(graph.neighbours(currentVertex)))
            {
                //recall last iteration's state
                const RecursionLevel& lower = callStack.top();
                currentVertex = lower.currentVertex;
                onePastCurrentIterator = lower.onePastCurrentIterator;
                callStack.pop();
            }

        } while(!callStack.empty());

        return GraphPath(path.begin(), path.end());
    }
}