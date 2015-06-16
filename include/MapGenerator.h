#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "../LibS/GeometryLight.h"
#include "../LibS/Array2.h"

#include <algorithm>
#include <functional>

class MapLayer;

class MapGenerator
{
public:
    MapGenerator(size_t width, size_t height);

    void generate(MapLayer& map);

protected:
    class TopologyMap
    {
    public:
        enum class TopologyState
        {
            Wall,
            Passage
        };

        TopologyMap(size_t width, size_t height) :
            m_automaton(CaveTopologyRules(), width, height, Geo::CellularAutomatonGridTopology::Toroidal),
            m_width(width),
            m_height(height)
        {

        }

        TopologyState at(size_t x, size_t y) const
        {
            return m_automaton.cellAt(x, y);
        }

        void iterate(size_t times)
        {
            m_automaton.iterate(times);
        }

    protected:
        class CaveTopologyRules
        {
        public:
            typedef TopologyState States;

            CaveTopologyRules() : iteration(0)
            {

            }

            States operator()(const Geo::CellularAutomaton<CaveTopologyRules>& automaton, size_t x, size_t y)
            {
                ++iteration;
                if(iteration < 5)
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
        protected:
            int iteration; //this should be added to parameter list for operator ()
        };
        Geo::CellularAutomaton<CaveTopologyRules> m_automaton;
        size_t m_width;
        size_t m_height;
    };

    class RegionMap //-1 is wall, 0.. are regions
    {
    public:
        RegionMap(size_t width, size_t height) :
            m_regionIds(width, height, -1),
            m_width(width),
            m_height(height)
        {

        }

        void updateFromTopologyMap(const TopologyMap& topologyMap)
        {
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

        int at(size_t x, size_t y) const
        {
            return m_regionIds(x, y);
        }

        static bool isWall(int value)
        {
            return value == -1;
        }

        size_t numberOfRegions() const
        {
            return m_numberOfRegions;
        }

        const Array2<int>& regionIds() const
        {
            return m_regionIds;
        }

    protected:
        Array2<int> m_regionIds;
        std::vector<int> m_regionsSizes;
        size_t m_numberOfRegions;
        size_t m_width;
        size_t m_height;
    };

    class RectangleMap
    {
    public:
        RectangleMap(size_t width, size_t height) :
            m_width(width),
            m_height(height)
        {

        }

        template <class F>
        void updateFromTopologyMap(const TopologyMap& topologyMap, F rectangleSizeAdjustingFunction)
        {
            for(int x = 0; x < m_width; ++x)
            {
                for(int y = 0; y < m_height; ++y)
                {
                    std::vector<Geo::Vec2I> localRects;
                    int xx = x;
                    int lastMaxY = m_height;
                    while(topologyMap.at(xx, y) == TopologyMap::TopologyState::Passage && xx < m_width)
                    {
                        int yy = y;
                        while(topologyMap.at(xx, yy) == TopologyMap::TopologyState::Passage && yy < lastMaxY)
                        {
                            ++yy;
                        }
                        lastMaxY = yy;
                        ++xx;

                        Geo::Vec2I size(xx - x, yy - y);
                        rectangleSizeAdjustingFunction(size);
                        if(size.y <= 0 || size.x <= 0) break;
                        localRects.push_back(size);
                    }

                    if(!localRects.empty())
                        m_rectangleSizes(x, y) = (*std::max_element(localRects.begin(), localRects.end(), [](const Geo::Vec2I & lhs, const Geo::Vec2I & rhs)->bool {return lhs.x * lhs.y < rhs.x* rhs.y;}));
                    else m_rectangleSizes(x, y) = Geo::Vec2I(0, 0);
                }
            }
        }

        const Geo::Vec2I& at(size_t x, size_t y) const
        {
            return m_rectangleSizes(x, y);
        }

    protected:
        Array2<Geo::Vec2I> m_rectangleSizes;
        size_t m_width;
        size_t m_height;
    };

    class MapFillingRectangles
    {
    public:
        MapFillingRectangles(size_t width, size_t height) :
            m_width(width),
            m_height(height)
        {

        }

        template <class F>
        void updateFromRegionMapAndRectangleMap(const RegionMap& regionMap, const RectangleMap& rectangleMap, F rectangleSizeAdjustingFunction)
        {
            auto intersects = [](const Geo::RectangleI & a, const Geo::RectangleI & b)->bool //local implementation to ensure that it checks for tangent boudaries
            {
                return a.max.x > b.min.x && a.min.x < b.max.x && a.max.y > b.min.y && a.min.y < b.max.y;
            };
            auto isRectangleValid = [](const Geo::RectangleI & r)->bool
            {
                return r.max.x > r.min.x && r.max.y > r.min.y;
            };

            enum class FillState
            {
                Wall,
                Unfilled,
                Filled
            };
            //auto rectangleSizeAdjustingFunction = [](Vec2I & size) {if(size.x > size.y * 2) size.x = size.y * 2; else if(size.y > size.x * 2) size.y = size.x * 2;};

            std::vector<std::vector<Geo::RectangleI>> rectanglesInRegion;
            rectanglesInRegion.resize(regionMap.numberOfRegions());

            std::vector<Geo::RectangleI> sortedRectangles;

            for(size_t x = 0; x < m_width; ++x)
            {
                for(size_t y = 0; y < m_height; ++y)
                {
                    Geo::Vec2I size = rectangleMap.at(x, y);
                    if(size.x <= 0 || size.y <= 0) continue;
                    Geo::RectangleI rectangle(Geo::Vec2I(x, y), size.x, size.y);
                    sortedRectangles.push_back(rectangle);
                }
            }

            std::sort(sortedRectangles.begin(), sortedRectangles.end(), [](const Geo::RectangleI & lhs, const Geo::RectangleI & rhs)->bool {return lhs.area() > rhs.area();}); //descending by area

            for(auto& rectangleToInsert : sortedRectangles)
            {
                std::vector<int> indicesOfIntersectingRectangles;
                int rectangleRegionId = regionMap.at(rectangleToInsert.min.x, rectangleToInsert.min.y);

                int i = 0;
                for(auto& rectangleInCurrentRegion : rectanglesInRegion[rectangleRegionId])
                {
                    if(intersects(rectangleInCurrentRegion, rectangleToInsert)) indicesOfIntersectingRectangles.push_back(i);
                    ++i;
                }

                std::sort(indicesOfIntersectingRectangles.begin(), indicesOfIntersectingRectangles.end(), [&rectanglesInRegion, &rectangleRegionId](int lhs, int rhs)->bool {return rectanglesInRegion[rectangleRegionId][lhs].area() > rectanglesInRegion[rectangleRegionId][rhs].area();});

                for(int rectangleIndex : indicesOfIntersectingRectangles) //this shrinks rectangles if they don't fit
                {
                    auto& intersectingRectangle = rectanglesInRegion[rectangleRegionId][rectangleIndex];

                    if(!intersects(intersectingRectangle, rectangleToInsert)) continue; //because it may not intersect anymore

                    if(intersectingRectangle.area() < rectangleToInsert.area())
                    {
                        Geo::RectangleI r1, r2; //for two ways of shrinking of the rectangle. One horizontally, one vertically.
                        r1 = r2 = intersectingRectangle;

                        if(r1.min.x < rectangleToInsert.min.x)
                        {
                            r1.max.x = rectangleToInsert.min.x;
                        }
                        else if(r1.max.x > rectangleToInsert.max.x)
                        {
                            r1.min.x = rectangleToInsert.max.x;
                        }
                        else
                        {
                            r1.max = r1.min;
                        }

                        if(r2.min.y < rectangleToInsert.min.y)
                        {
                            r2.max.y = rectangleToInsert.min.y;
                        }
                        else if(r2.max.y > rectangleToInsert.max.y)
                        {
                            r2.min.y = rectangleToInsert.max.y;
                        }
                        else
                        {
                            r2.max = r2.min;
                        }

                        Geo::Vec2I size1(r1.width(), r1.height());
                        rectangleSizeAdjustingFunction(size1);
                        r1.max = r1.min + size1;

                        Geo::Vec2I size2(r2.width(), r2.height());
                        rectangleSizeAdjustingFunction(size2);
                        r2.max = r2.min + size2;

                        if(r1.area() > r2.area()) intersectingRectangle = r1;
                        else intersectingRectangle = r2;
                    }
                    else
                    {
                        Geo::RectangleI r1, r2;
                        r1 = r2 = rectangleToInsert;

                        if(r1.min.x < intersectingRectangle.min.x)
                        {
                            r1.max.x = intersectingRectangle.min.x;
                        }
                        else if(r1.max.x > intersectingRectangle.max.x)
                        {
                            r1.min.x = intersectingRectangle.max.x;
                        }
                        else
                        {
                            r1.max = r1.min;
                        }

                        if(r2.min.y < intersectingRectangle.min.y)
                        {
                            r2.max.y = intersectingRectangle.min.y;
                        }
                        else if(r2.max.y > intersectingRectangle.max.y)
                        {
                            r2.min.y = intersectingRectangle.max.y;
                        }
                        else
                        {
                            r2.max = r2.min;
                        }

                        Geo::Vec2I size1(r1.width(), r1.height());
                        rectangleSizeAdjustingFunction(size1);
                        r1.max = r1.min + size1;

                        Geo::Vec2I size2(r2.width(), r2.height());
                        rectangleSizeAdjustingFunction(size2);
                        r2.max = r2.min + size2;

                        if(r1.area() > r2.area()) rectangleToInsert = r1;
                        else rectangleToInsert = r2;

                        if(!isRectangleValid(rectangleToInsert)) break; //rectangle is invalid
                    }

                }
                rectanglesInRegion[rectangleRegionId].push_back(rectangleToInsert);
                rectanglesInRegion[rectangleRegionId].erase(std::remove_if(rectanglesInRegion[rectangleRegionId].begin(), rectanglesInRegion[rectangleRegionId].end(), std::not1(isRectangleValid)), rectanglesInRegion[rectangleRegionId].end());
            }

            Array2<int> rectangleFillMap;
            for(size_t x = 0; x < m_width; ++x)
            {
                for(size_t y = 0; y < m_height; ++y)
                {
                    if(RegionMap::isWall(regionMap.at(x, y)))
                        rectangleFillMap(x, y) = FillState::Wall;
                    else
                        rectangleFillMap(x, y) = FillState::Unfilled; //unfilled
                }
            }

            for(const auto& rectangles : rectanglesInRegion)
            {
                for(const auto& rectangle : rectangles)
                {
                    for(size_t x = rectangle.min.x; x < rectangle.max.x; ++x)
                    {
                        for(size_t y = rectangle.min.y; y < rectangle.max.y; ++y)
                        {
                            rectangleFillMap(x, y) = FillState::Filled;
                        }
                    }
                }
            }

            for(size_t x = 0; x < m_width; ++x)
            {
                for(size_t y = 0; y < m_height; ++y)
                {
                    if(rectangleFillMap(x, y) == FillState::Unfilled)
                    {
                        rectanglesInRegion[regionMap.at(x, y)].push_back(Geo::RectangleI(Geo::Vec2I {x, y}, 1, 1));
                    }
                }
            }

            m_rectangles.clear();
            for(auto& rectangles : rectanglesInRegion)
            {
                m_rectangles.insert(m_rectangles.end(), rectangles.begin(), rectangles.end());
            }
        }

    protected:
        std::vector<Geo::RectangleI> m_rectangles;
        size_t m_width;
        size_t m_height;
    };

    TopologyMap m_topologyMap;
    RegionMap m_regionMap;
    RectangleMap m_rectangleMap;
    MapFillingRectangles m_rectangles;
    size_t m_width;
    size_t m_height;
};

#endif // MAPGENERATOR_H
