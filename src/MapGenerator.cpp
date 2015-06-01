#include "MapGenerator.h"

#include "Tile.h"

#include "TileStack.h"
#include "MapLayer.h"

#include "ResourceManager.h"

MapGenerator::MapGenerator()
{

}

void MapGenerator::generate(MapLayer& map)
{
    ResourceHandle<Tile> floor = ResourceManager::instance().get<Tile>("Test Tile");
    for(auto& tileStack : map.tileStacks())
    {
        tileStack.push(floor.get().clone().release());
    }
}
