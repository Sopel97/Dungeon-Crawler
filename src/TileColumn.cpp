#include "TileColumn.h"

#include "tiles/TileStack.h"
#include "tiles/Tile.h"

#include "tiles/models/TileModel.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/controllers/TileController.h"

using namespace ls;

const Rectangle2F TileColumn::m_defaultCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));

TileStack TileColumn::m_emptyTile(std::make_unique<Tile>(-1, std::make_unique<TileModel>(), std::make_unique<TileRenderer>(), std::make_unique<TileController>()), 1);

TileColumn::TileColumn() :
    m_lastTile(-1)
{

}

TileColumn::~TileColumn()
{
    for(auto tile : m_tiles)
    {
        delete tile;
    }
}

const TileStack& TileColumn::top() const
{
    return *(m_tiles[m_lastTile]);
}
TileStack& TileColumn::top()
{
    return *(m_tiles[m_lastTile]);
}

void TileColumn::push(TileStack* tile)
{
    m_tiles.push_back(tile);
    ++m_lastTile;
}

TileStack* TileColumn::releaseTop()
{
    TileStack* tileStack = m_tiles[m_lastTile];
    m_tiles.pop_back();
    --m_lastTile;
    return tileStack;
}

void TileColumn::deleteTop()
{
    delete releaseTop();
}
void TileColumn::deleteAt(size_t z)
{
    if(z >= m_tiles.size()) return;
    delete m_tiles[z];
    m_tiles.erase(m_tiles.begin() + z);
}

int TileColumn::insert(TileStack* tileStack, int count)
{
    if(count == -1) count = std::numeric_limits<int>::max();
    int numberOfInsertedTiles = 0;

    size_t z = 0;
    while(z < m_tiles.size() && numberOfInsertedTiles < count)
    {
        numberOfInsertedTiles += insert(tileStack, z, count - numberOfInsertedTiles);
    }

    return numberOfInsertedTiles;
}
int TileColumn::insert(TileStack* tileStack, size_t slotId, int count)
{
    if(slotId >= m_tiles.size()) return 0;

    if(count == -1) count = std::numeric_limits<int>::max();
    if(count <= 0) return 0;

    TileStack* storedTile = m_tiles[slotId];
    if(storedTile == nullptr)
    {
        storedTile = tileStack->clone().release();
        storedTile->setQuantity(0); //after inserting it won't be 0
    }
    else if(!storedTile->tile().equals(tileStack->tile()))
    {
        return 0;
    }

    int storedTileQuantity = storedTile->quantity();
    int freeSpace = storedTile->maxQuantity() - storedTileQuantity;

    count = std::min(count, freeSpace);

    int newQuantity = storedTileQuantity + count;
    storedTile->setQuantity(newQuantity);

    return count;
}

int TileColumn::erase(TileStack* tileStack, int count)
{
    if(count == -1) count = std::numeric_limits<int>::max();
    int numberOfErasedTiles = 0;

    size_t z = 0;
    while(z < m_tiles.size() && numberOfErasedTiles < count)
    {
        numberOfErasedTiles += erase(tileStack, z, count - numberOfErasedTiles);
    }
    
    return numberOfErasedTiles;
}
int TileColumn::erase(TileStack* tileStack, size_t slotId, int count)
{
    if(slotId >= m_tiles.size()) return 0;

    TileStack* storedTile = m_tiles[slotId];
    if(storedTile == nullptr) return 0;
    if(!storedTile->tile().equals(tileStack->tile())) return 0;
    int storedTileQuantity = storedTile->quantity();

    if(count == -1) count = storedTileQuantity;
    int newQuantity = storedTileQuantity - count;
    if(newQuantity <= 0)
    {
        deleteAt(slotId);
    }
    else
    {
        storedTile->setQuantity(newQuantity);
    }

    return count;
}

const std::vector<TileStack*>& TileColumn::tiles() const
{
    return m_tiles;
}

bool TileColumn::isValid(int z) const
{
    return m_lastTile >= z;
}

const TileStack& TileColumn::at(int z) const
{
    if(isValid(z)) return *(m_tiles[z]);
    return m_emptyTile;
}
TileStack& TileColumn::at(int z)
{
    if(isValid(z)) return *(m_tiles[z]);
    return m_emptyTile;
}
int TileColumn::size() const
{
    return m_lastTile + 1;
}

bool TileColumn::isTall() const
{
    for(const TileStack* tileStack : m_tiles)
        if(tileStack->tile().renderer().isTall()) return true;

    return false;
}

bool TileColumn::hasCollider() const
{
    for(const TileStack* tileStack : m_tiles)
    {
        if(tileStack->tile().model().hasCollider()) return true;
    }

    return false;
}
const Rectangle2F& TileColumn::collider() const
{
    for(const TileStack* tileStack : m_tiles)
    {
        if(tileStack->tile().model().hasCollider()) return tileStack->tile().model().collider();
    }

    return m_defaultCollider;
}

int TileColumn::topZ() const
{
    return m_lastTile;
}
