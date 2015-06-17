#include "MapGenerator.h"

#include "Tile.h"

#include "TileStack.h"
#include "MapLayer.h"

#include "ResourceManager.h"

#include "Root.h"

#include "../LibS/GeometryLight.h"

#include <cstdlib>

using namespace Geo;

MapGenerator::MapGenerator(size_t width, size_t height) :
    m_topologyMap(width, height),
    m_regionMap(width, height),
    m_rectangleMap(width, height),
    m_rectangles(width, height),
    m_width(width),
    m_height(height)
{

}

void MapGenerator::updateHelperMaps()
{
    auto rectangleSizeAdjustingFunction = [](Vec2I & size)
    {
        if(size.x > size.y * 2) size.x = size.y * 2;
        else if(size.y > size.x * 2) size.y = size.x * 2;
    };

    m_regionMap.updateFromTopologyMap(m_topologyMap);
    m_rectangleMap.updateFromTopologyMap(m_topologyMap, rectangleSizeAdjustingFunction);
    m_rectangles.updateFromRegionMapAndRectangleMap(m_regionMap, m_rectangleMap, rectangleSizeAdjustingFunction);
}

std::vector<LineSegmentF> MapGenerator::produceConnectionsBetweenDisconnectedRegions()
{
    constexpr int minRectSize = 4; //will reduce the number of rectangles being triangulated

    int numberOfRegions = m_regionMap.numberOfRegions();

    if(numberOfRegions <= 1) return {};

    std::vector<Vec2F> centersOfRectangles;
    for(const auto& rectangle : m_rectangles.rectangles())
    {
        if(rectangle.area() >= minRectSize)
            centersOfRectangles.push_back((Vec2F(rectangle.min) + Vec2F(rectangle.max)) / 2.0f);
    }

    PointSetDelaunayTriangulationF triangulation(centersOfRectangles);

    std::vector<LineSegmentF> connections;
    std::vector<std::pair<int, int>> edges;
    std::vector<std::pair<int, int>> finalEdges;

    for(const auto& connection : triangulation.connections())
    {
        Vec2F a = triangulation.point(connection.i);
        Vec2F b = triangulation.point(connection.j);
        edges.emplace_back(connection.i, connection.j);
        //connections.emplace_back(a, b);
    }

    std::sort(edges.begin(), edges.end(), [&centersOfRectangles](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs) {return centersOfRectangles[lhs.first].distanceTo(centersOfRectangles[lhs.second]) < centersOfRectangles[rhs.first].distanceTo(centersOfRectangles[rhs.second]);});

    std::vector<std::set<int>> unions;
    for(int i = 0; i < numberOfRegions; ++i)
    {
        unions.emplace_back(std::initializer_list<int> {i});
    }

    int currentEdge = 0;
    while(currentEdge < edges.size() && unions[0].size() < numberOfRegions)
    {
        int a = edges[currentEdge].first;
        int b = edges[currentEdge].second;

        int ra = m_regionMap.at(centersOfRectangles[a].x, centersOfRectangles[a].y);
        int rb = m_regionMap.at(centersOfRectangles[b].x, centersOfRectangles[b].y);

        ++currentEdge;

        if(ra == rb) continue;

        int ai = -1;
        int bi = -1;

        for(int i = 0; i < unions.size(); ++i)
        {
            if(unions[i].find(ra) != unions[i].end())
            {
                ai = i;
                break;
            }
        }
        for(int i = 0; i < unions.size(); ++i)
        {
            if(unions[i].find(rb) != unions[i].end())
            {
                bi = i;
                break;
            }
        }

        if(ai != bi)
        {
            if(ai > bi) std::swap(ai, bi);

            unions[ai].insert(unions[bi].begin(), unions[bi].end());
            unions[bi].clear();

            finalEdges.emplace_back(a, b);
        }
    }

    for(const auto& edge : finalEdges)
    {
        Vec2F a = triangulation.point(edge.first);
        Vec2F b = triangulation.point(edge.second);
        connections.emplace_back(a, b);
    }

    return connections;
}
void MapGenerator::removeSmallRegionsFromTopologyMap()
{
    constexpr int minRegionSize = 100;
    constexpr float minMapFill = 0.4f;

    std::vector<std::pair<int, int>> sizesByRegion;
    for(int i = 0; i < m_regionMap.numberOfRegions(); ++i)
    {
        sizesByRegion.emplace_back(i, m_regionMap.regionSize(i));
    }

    std::sort(sizesByRegion.begin(), sizesByRegion.end(), [](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)->bool {return lhs.second < rhs.second;});

    int totalBoardSize = m_width * m_height;
    int totalGroundSize = m_topologyMap.countStates(TopologyMap::TopologyState::Passage);
    for(int i = 0; i < sizesByRegion.size(); ++i)
    {
        int currentRegionId = sizesByRegion[i].first;
        int currentRegionSize = sizesByRegion[i].second;

        if((totalGroundSize - currentRegionSize) / (float)totalBoardSize < minMapFill || currentRegionSize > minRegionSize) break;

        for(int x = 0; x < m_width; ++x)
        {
            for(int y = 0; y < m_height; ++y)
            {
                if(m_regionMap.at(x, y) == currentRegionId) m_topologyMap.setState(x, y, TopologyMap::TopologyState::Wall);
            }
        }

        totalGroundSize -= currentRegionSize;
    }
}
void MapGenerator::prepareHelperMaps()
{
    constexpr size_t passes = 3u;
    for(int pass = 0; pass < passes; ++pass)
    {
        m_topologyMap.iterate(7);
        m_regionMap.updateFromTopologyMap(m_topologyMap);
        removeSmallRegionsFromTopologyMap();
        updateHelperMaps();

        if(pass < passes - 1) //is not the last pass
        {
            if(m_regionMap.numberOfRegions() > 1)
            {
                auto newConnections = produceConnectionsBetweenDisconnectedRegions();
                if(newConnections.empty()) return;
                m_topologyMap.reset();

                for(const auto& line : newConnections)
                {
                    Vec2F lineVector = line.end - line.begin;
                    Vec2F dir = lineVector.normalized();
                    Vec2F normal = dir.normal();
                    float length = lineVector.magnitude();
                    for(float i = 0.0f; i < length; i += 0.5f)
                    {
                        for(float j = -1.5f; j < 1.6f; j += 0.5f)
                        {
                            Vec2F pos = line.begin + dir * i + normal * j;
                            int x = std::round(pos.x);
                            int y = std::round(pos.y);
                            if(x < 0 || y < 0 || x >= m_width || y >= m_height) continue;
                            m_topologyMap.setState(x, y, TopologyMap::TopologyState::Passage);
                        }
                    }

                }
            }
            else
            {
                return;
            }
        }

    }
}
void MapGenerator::generate(MapLayer& map)
{
    ResourceHandle<Tile> floorTile = ResourceManager::instance().get<Tile>("Test Floor Tile");
    ResourceHandle<Tile> wallTile = ResourceManager::instance().get<Tile>("Test Wall Tile");

    prepareHelperMaps();

    for(size_t x = 0; x < m_width; ++x)
    {
        for(size_t y = 0; y < m_height; ++y)
        {
            auto& tileStack = map.at(x, y);
            tileStack.push(floorTile.get().clone().release());
            if(m_topologyMap.at(x, y) == TopologyMap::TopologyState::Wall)
                tileStack.push(wallTile.get().clone().release());
        }
    }

}
