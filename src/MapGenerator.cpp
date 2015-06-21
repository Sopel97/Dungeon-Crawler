#include "MapGenerator.h"

#include "Tile.h"
#include "TileView.h"

#include "TileStack.h"
#include "MapLayer.h"

#include "ResourceManager.h"

#include "Root.h"

#include "../LibS/GeometryLight.h"

#include <cstdlib>

using namespace Geo;

/*
    Helper classes
*/
MapGenerator::TopologyMap::TopologyMap(size_t width, size_t height) :
    m_automaton(CaveTopologyRules(), width, height, Geo::CellularAutomatonGridTopology::Toroidal),
    m_initialAutomatonState(CaveTopologyRules(), width, height, Geo::CellularAutomatonGridTopology::Toroidal),
    m_width(width),
    m_height(height)
{
    m_automaton.fill([width, height](size_t x, size_t y) -> TopologyState
    {
        if(width - x <= 2 || x <= 1 || height - y <= 2 || y <= 1) return TopologyState::Wall;
        return (rand() / (float)RAND_MAX < 0.5 ? TopologyState::Wall : TopologyState::Passage);
    });
    m_initialAutomatonState = m_automaton;
}

MapGenerator::TopologyMap::TopologyState MapGenerator::TopologyMap::at(size_t x, size_t y) const
{
    return m_automaton.cellAt(x, y);
}

void MapGenerator::TopologyMap::setState(size_t x, size_t y, MapGenerator::TopologyMap::TopologyState newState)
{
    m_automaton.setCell(x, y, newState);
}

void MapGenerator::TopologyMap::iterate(size_t times)
{
    m_automaton.iterate(times);
}

size_t MapGenerator::TopologyMap::countStates(MapGenerator::TopologyMap::TopologyState state)
{
    size_t count = 0;
    for(size_t x = 0; x < m_width; ++x)
    {
        for(size_t y = 0; y < m_height; ++y)
        {
            if(m_automaton.cellAt(x, y) == state) ++count;
        }
    }
    return count;
}

void MapGenerator::TopologyMap::reset()
{
    m_automaton = m_initialAutomatonState;
}

MapGenerator::TopologyMap::CaveTopologyRules::CaveTopologyRules() : m_iteration(0)
{

}

MapGenerator::TopologyMap::CaveTopologyRules::States MapGenerator::TopologyMap::CaveTopologyRules::operator()(const Geo::CellularAutomaton<MapGenerator::TopologyMap::CaveTopologyRules>& automaton, size_t x, size_t y)
{
    ++m_iteration;
    if(m_iteration < 5)
    {
        if(automaton.quantityOfStateIn3x3(States::Wall, x, y) >= 5 || automaton.quantityOfStateIn5x5(States::Wall, x, y) <= 2) return States::Wall;
        else return States::Passage;
    }
    else
    {
        if(automaton.quantityOfStateIn3x3(States::Wall, x, y) >= 5) return States::Wall;
        else return States::Passage;
    }
}

MapGenerator::RegionMap::RegionMap(size_t width, size_t height) :
    m_regionIds(width, height, -1),
    m_width(width),
    m_height(height)
{

}

void MapGenerator::RegionMap::updateFromTopologyMap(const MapGenerator::TopologyMap& topologyMap)
{
    m_regionIds.fill(-1);

    for(size_t x = 0; x < m_width; ++x)
    {
        for(size_t y = 0; y < m_height; ++y)
        {
            if(topologyMap.at(x, y) == TopologyMap::TopologyState::Passage) m_regionIds(x, y) = 0;
        }
    }

    int currentRegionId = 0;
    for(size_t x = 0; x < m_width; ++x)
    {
        for(size_t y = 0; y < m_height; ++y)
        {
            if(m_regionIds(x, y) == 0) m_regionIds.floodFill(x, y, currentRegionId++);
        }
    }
    m_numberOfRegions = currentRegionId;

    m_regionsSizes.clear();
    m_regionsSizes.resize(currentRegionId);

    for(const auto& c : m_regionIds)
    {
        if(!isWall(c))
            ++m_regionsSizes[c];
    }

}

int MapGenerator::RegionMap::at(size_t x, size_t y) const
{
    return m_regionIds(x, y);
}

bool MapGenerator::RegionMap::isWall(int value)
{
    return value == -1;
}

size_t MapGenerator::RegionMap::numberOfRegions() const
{
    return m_numberOfRegions;
}

const Array2<int>& MapGenerator::RegionMap::regionIds() const
{
    return m_regionIds;
}

size_t MapGenerator::RegionMap::regionSize(size_t regionsId) const
{
    return m_regionsSizes[regionsId];
}


MapGenerator::RectangleMap::RectangleMap(size_t width, size_t height) :
    m_rectangleSizes(width, height, Geo::Vec2I(0, 0)),
    m_width(width),
    m_height(height)
{

}


const Geo::Vec2I& MapGenerator::RectangleMap::at(size_t x, size_t y) const
{
    return m_rectangleSizes(x, y);
}

MapGenerator::MapFillingRectangles::MapFillingRectangles(size_t width, size_t height) :
    m_width(width),
    m_height(height)
{

}


const std::vector<Geo::RectangleI>& MapGenerator::MapFillingRectangles::rectangles() const
{
    return m_rectangles;
}

/*
    End of helper classes
*/


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

    size_t numberOfRegions = m_regionMap.numberOfRegions();

    if(numberOfRegions <= 1) return {};

    std::vector<Vec2F> centersOfRectangles;
    for(const auto& rectangle : m_rectangles.rectangles())
    {
        if(rectangle.area() >= minRectSize)
            centersOfRectangles.push_back((Vec2F(rectangle.min) + Vec2F(rectangle.max)) / 2.0f);
    }

    PointSetDelaunayTriangulationF triangulation(centersOfRectangles);

    std::vector<LineSegmentF> connections;
    std::vector<std::pair<size_t, size_t>> edges;
    std::vector<std::pair<size_t, size_t>> finalEdges;

    for(const auto& connection : triangulation.connections())
    {
        Vec2F a = triangulation.point(connection.i);
        Vec2F b = triangulation.point(connection.j);
        edges.emplace_back(connection.i, connection.j);
        //connections.emplace_back(a, b);
    }

    std::sort(edges.begin(), edges.end(), [&centersOfRectangles](const std::pair<size_t, size_t>& lhs, const std::pair<size_t, size_t>& rhs) {return centersOfRectangles[lhs.first].distanceTo(centersOfRectangles[lhs.second]) < centersOfRectangles[rhs.first].distanceTo(centersOfRectangles[rhs.second]);});

    std::vector<std::set<size_t>> unions;
    for(size_t i = 0; i < numberOfRegions; ++i)
    {
        unions.emplace_back(std::initializer_list<size_t> {i});
    }

    size_t currentEdge = 0;
    while(currentEdge < edges.size() && unions[0].size() < numberOfRegions)
    {
        size_t a = edges[currentEdge].first;
        size_t b = edges[currentEdge].second;

        size_t ra = m_regionMap.at(centersOfRectangles[a].x, centersOfRectangles[a].y);
        size_t rb = m_regionMap.at(centersOfRectangles[b].x, centersOfRectangles[b].y);

        ++currentEdge;

        if(ra == rb) continue;

        size_t ai = 0u;
        size_t bi = 0u;

        for(size_t i = 0; i < unions.size(); ++i)
        {
            if(unions[i].find(ra) != unions[i].end())
            {
                ai = i;
                break;
            }
        }
        for(size_t i = 0; i < unions.size(); ++i)
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
    for(size_t i = 0; i < m_regionMap.numberOfRegions(); ++i)
    {
        sizesByRegion.emplace_back(i, m_regionMap.regionSize(i));
    }

    std::sort(sizesByRegion.begin(), sizesByRegion.end(), [](const std::pair<int, int>& lhs, const std::pair<int, int>& rhs)->bool {return lhs.second < rhs.second;});

    int totalBoardSize = m_width * m_height;
    int totalGroundSize = m_topologyMap.countStates(TopologyMap::TopologyState::Passage);
    for(size_t i = 0; i < sizesByRegion.size(); ++i)
    {
        int currentRegionId = sizesByRegion[i].first;
        int currentRegionSize = sizesByRegion[i].second;

        if((totalGroundSize - currentRegionSize) / (float)totalBoardSize < minMapFill || currentRegionSize > minRegionSize) break;

        for(size_t x = 0; x < m_width; ++x)
        {
            for(size_t y = 0; y < m_height; ++y)
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
    for(size_t pass = 0; pass < passes; ++pass)
    {
        m_topologyMap.iterate(7);
        m_regionMap.updateFromTopologyMap(m_topologyMap);
        removeSmallRegionsFromTopologyMap();
        updateHelperMaps();

        if(pass < passes - 1u) //is not the last pass
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
                            size_t x = std::round(pos.x);
                            size_t y = std::round(pos.y);
                            if(x < 0u || y < 0u || x >= m_width || y >= m_height) continue;
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
    ResourceHandle<Tile> floorTile2 = ResourceManager::instance().get<Tile>("Test Floor Tile2");
    ResourceHandle<Tile> wallTile = ResourceManager::instance().get<Tile>("Test Wall Tile");

    prepareHelperMaps();

    for(size_t x = 0; x < m_width; ++x)
    {
        for(size_t y = 0; y < m_height; ++y)
        {
            //auto& floorTileToPlace = rand()/(float)RAND_MAX < 0.5f ? floorTile : floorTile2;
            auto& floorTileToPlace = floorTile;

            auto& tileStack = map.at(x, y);
            tileStack.push(floorTileToPlace.get().clone().release());

            if(m_topologyMap.at(x, y) == TopologyMap::TopologyState::Wall)
                tileStack.push(wallTile.get().clone().release());
        }
    }

}
