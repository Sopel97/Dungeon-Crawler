#include "MapGenerator.h"

#include "Tile.h"

#include "TileStack.h"
#include "MapLayer.h"

#include "ResourceManager.h"

#include "Root.h"

#include "../LibS/GeometryLight.h"

#include <cstdlib>

using namespace Geo;

MapGenerator::MapGenerator()
{

}

void MapGenerator::generate(MapLayer& map)
{
    size_t width = map.width();
    size_t height = map.height();

    ResourceHandle<Tile> floor = ResourceManager::instance().get<Tile>("Test Floor Tile");
    ResourceHandle<Tile> wall = ResourceManager::instance().get<Tile>("Test Wall Tile");

    //white floor, black wall
    CellularAutomaton<QuantityRules<OriginalCellularAutomatonStates>> ca(
                QuantityRules<OriginalCellularAutomatonStates>(
                    OriginalCellularAutomatonStates::Black,
                    std::array<OriginalCellularAutomatonStates, 10u>
    {
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::White,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black,
        OriginalCellularAutomatonStates::Black
    }),
    width, height);
    ca.fill([width, height](size_t x, size_t y) -> OriginalCellularAutomatonStates
    {
        if(width - x <= 2 || x <= 1 || height - y <= 2 || y <= 1) return OriginalCellularAutomatonStates::Black;
        return (rand() / (float)RAND_MAX < 0.5 ? OriginalCellularAutomatonStates::Black : OriginalCellularAutomatonStates::White);
    });

    ca.iterate(5);

    for(size_t x = 0; x < width; ++x)
    {
        for(size_t y = 0; y < height; ++y)
        {
            auto& tileStack = map.at(x, y);
            tileStack.push(floor.get().clone().release());
            if(ca.cellAt(x, y) == OriginalCellularAutomatonStates::Black)
                tileStack.push(wall.get().clone().release());
        }
    }

}
