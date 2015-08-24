#ifndef MAPGENERATOR_H
#define MAPGENERATOR_H

#include "../LibS/Geometry.h"
#include "../LibS/Array2.h"

#include <algorithm>
#include <functional>

class MapLayer;

class MapGenerator
{
public:
    MapGenerator(size_t width, size_t height);

    void updateHelperMaps();
    std::vector<ls::LineSegmentF> produceConnectionsBetweenDisconnectedRegions();
    void removeSmallRegionsFromTopologyMap(); //does not update
    void prepareHelperMaps();

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

        TopologyMap(size_t width, size_t height);

        TopologyState at(size_t x, size_t y) const;

        void setState(size_t x, size_t y, TopologyState newState);

        void iterate(size_t times);

        size_t countStates(TopologyState state);

        void reset();

    protected:
        class CaveTopologyRules
        {
        public:
            typedef TopologyState States;

            CaveTopologyRules();

            States operator()(const ls::CellularAutomaton<CaveTopologyRules>& automaton, size_t x, size_t y);
        protected:
            int m_iteration; //this should be added to parameter list for operator ()
        };
        ls::CellularAutomaton<CaveTopologyRules> m_automaton;
        ls::CellularAutomaton<CaveTopologyRules> m_initialAutomatonState;
        size_t m_width;
        size_t m_height;
    };

    class RegionMap //-1 is wall, 0.. are regions
    {
    public:
        RegionMap(size_t width, size_t height);

        void updateFromTopologyMap(const TopologyMap& topologyMap);

        int at(size_t x, size_t y) const;

        static bool isWall(int value);

        size_t numberOfRegions() const;

        const Array2<int>& regionIds() const;

        size_t regionSize(size_t regionsId) const;

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
        RectangleMap(size_t width, size_t height);

        template <class F>
        void updateFromTopologyMap(const TopologyMap& topologyMap, F rectangleSizeAdjustingFunction)
        {
            for(size_t x = 0; x < m_width; ++x)
            {
                for(size_t y = 0; y < m_height; ++y)
                {
                    std::vector<ls::Vec2I> localRects;
                    size_t xx = x;
                    size_t lastMaxY = m_height;
                    while(topologyMap.at(xx, y) == TopologyMap::TopologyState::Passage && xx < m_width)
                    {
                        size_t yy = y;
                        while(topologyMap.at(xx, yy) == TopologyMap::TopologyState::Passage && yy < lastMaxY)
                        {
                            ++yy;
                        }
                        lastMaxY = yy;
                        ++xx;

                        ls::Vec2I size(xx - x, yy - y);
                        rectangleSizeAdjustingFunction(size);
                        if(size.y <= 0 || size.x <= 0) break;
                        localRects.push_back(size);
                    }

                    if(!localRects.empty())
                        m_rectangleSizes(x, y) = (*std::max_element(localRects.begin(), localRects.end(), [](const ls::Vec2I & lhs, const ls::Vec2I & rhs)->bool {return lhs.x * lhs.y < rhs.x* rhs.y;}));
                    else m_rectangleSizes(x, y) = ls::Vec2I(0, 0);
                }
            }
        }
        const ls::Vec2I& at(size_t x, size_t y) const;

    protected:
        Array2<ls::Vec2I> m_rectangleSizes;
        size_t m_width;
        size_t m_height;
    };

    class MapFillingRectangles
    {
    public:
        MapFillingRectangles(size_t width, size_t height);

        template <class F>
        void updateFromRegionMapAndRectangleMap(const RegionMap& regionMap, const RectangleMap& rectangleMap, F rectangleSizeAdjustingFunction)
        {
            auto intersects = [](const ls::RectangleI & a, const ls::RectangleI & b)->bool //local implementation to ensure that it checks for tangent boudaries
            {
                return a.max.x > b.min.x && a.min.x < b.max.x && a.max.y > b.min.y && a.min.y < b.max.y;
            };
            auto isRectangleValid = [](const ls::RectangleI & r)->bool
            {
                return r.max.x > r.min.x && r.max.y > r.min.y;
            };
            auto isRectangleNotValid = [&isRectangleValid](const ls::RectangleI & r)->bool
            {
                return !isRectangleValid;
            };

            enum class FillState
            {
                Wall,
                Unfilled,
                Filled
            };
            //auto rectangleSizeAdjustingFunction = [](Vec2I & size) {if(size.x > size.y * 2) size.x = size.y * 2; else if(size.y > size.x * 2) size.y = size.x * 2;};

            std::vector<std::vector<ls::RectangleI>> rectanglesInRegion;
            rectanglesInRegion.resize(regionMap.numberOfRegions());

            std::vector<ls::RectangleI> sortedRectangles;

            for(size_t x = 0; x < m_width; ++x)
            {
                for(size_t y = 0; y < m_height; ++y)
                {
                    ls::Vec2I size = rectangleMap.at(x, y);
                    if(size.x <= 0 || size.y <= 0) continue;
                    ls::RectangleI rectangle(ls::Vec2I(x, y), size.x, size.y);
                    sortedRectangles.push_back(rectangle);
                }
            }

            std::sort(sortedRectangles.begin(), sortedRectangles.end(), [](const ls::RectangleI & lhs, const ls::RectangleI & rhs)->bool {return lhs.area() > rhs.area();}); //descending by area

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
                        ls::RectangleI r1, r2; //for two ways of shrinking of the rectangle. One horizontally, one vertically.
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

                        ls::Vec2I size1(r1.width(), r1.height());
                        rectangleSizeAdjustingFunction(size1);
                        r1.max = r1.min + size1;

                        ls::Vec2I size2(r2.width(), r2.height());
                        rectangleSizeAdjustingFunction(size2);
                        r2.max = r2.min + size2;

                        if(r1.area() > r2.area()) intersectingRectangle = r1;
                        else intersectingRectangle = r2;
                    }
                    else
                    {
                        ls::RectangleI r1, r2;
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

                        ls::Vec2I size1(r1.width(), r1.height());
                        rectangleSizeAdjustingFunction(size1);
                        r1.max = r1.min + size1;

                        ls::Vec2I size2(r2.width(), r2.height());
                        rectangleSizeAdjustingFunction(size2);
                        r2.max = r2.min + size2;

                        if(r1.area() > r2.area()) rectangleToInsert = r1;
                        else rectangleToInsert = r2;

                        if(!isRectangleValid(rectangleToInsert)) break; //rectangle is invalid
                    }

                }
                rectanglesInRegion[rectangleRegionId].push_back(rectangleToInsert);
                rectanglesInRegion[rectangleRegionId].erase(std::remove_if(rectanglesInRegion[rectangleRegionId].begin(), rectanglesInRegion[rectangleRegionId].end(), isRectangleNotValid), rectanglesInRegion[rectangleRegionId].end());
            }

            Array2<FillState> rectangleFillMap(m_width, m_height);
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
                    for(int x = rectangle.min.x; x < rectangle.max.x; ++x)
                    {
                        for(int y = rectangle.min.y; y < rectangle.max.y; ++y)
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
                        rectanglesInRegion[regionMap.at(x, y)].push_back(ls::RectangleI(ls::Vec2I(x, y), 1, 1));
                    }
                }
            }

            m_rectangles.clear();
            for(auto& rectangles : rectanglesInRegion)
            {
                m_rectangles.insert(m_rectangles.end(), rectangles.begin(), rectangles.end());
            }
        }

        const std::vector<ls::RectangleI>& rectangles() const;

    protected:
        std::vector<ls::RectangleI> m_rectangles;
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
