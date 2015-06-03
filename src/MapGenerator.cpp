#include "MapGenerator.h"

#include "Tile.h"

#include "TileStack.h"
#include "MapLayer.h"

#include "ResourceManager.h"

#include "Root.h"

MapGenerator::MapGenerator()
{

}

void MapGenerator::generate(MapLayer& map)
{
    ResourceHandle<Tile> floor = ResourceManager::instance().get<Tile>("Test Floor Tile");
    ResourceHandle<Tile> wall = ResourceManager::instance().get<Tile>("Test Wall Tile");
    for(auto& tileStack : map.tileStacks())
    {
        tileStack.push(floor.get().clone().release());
        if(Root::instance().rng().nextBool())
        {
            tileStack.push(wall.get().clone().release());
        }
    }
}
