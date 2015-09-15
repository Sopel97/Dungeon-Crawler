#include "TileStack.h"

#include "tiles/Tile.h"

#include "tiles/models/TileModel.h"
#include "tiles/views/TileView.h"
#include "tiles/controllers/TileController.h"

using namespace ls;

const RectangleF TileStack::m_defaultCollider(Vec2F(0.0f, 0.0f), Vec2F(32.0f, 32.0f));

Tile TileStack::m_emptyTile {std::make_unique<TileModel>(nullptr), std::make_unique<TileView>(nullptr), std::make_unique<TileController>(nullptr)};

TileStack::TileStack() :
    m_lastTile(-1)
{

}

TileStack::~TileStack()
{
    for(auto tile : m_tiles)
    {
        delete tile;
    }
}

const Tile& TileStack::top() const
{
    return *(m_tiles[m_lastTile]);
}
Tile& TileStack::top()
{
    return *(m_tiles[m_lastTile]);
}

void TileStack::push(Tile* tile)
{
    m_tiles.push_back(tile);
    ++m_lastTile;
}

Tile* TileStack::releaseTop()
{
    Tile* tile = m_tiles[m_lastTile];
    m_tiles.erase(m_tiles.begin() + m_lastTile);
    --m_lastTile;
    return tile;
}

void TileStack::deleteTop()
{
    delete releaseTop();
}
void TileStack::deleteAt(size_t z)
{
    if(z >= m_tiles.size()) return;
    delete m_tiles[z];
    m_tiles.erase(m_tiles.begin() + z);
}

int TileStack::insert(Tile* tile, int count)
{
    if(count == -1) count = std::numeric_limits<int>::max();
    int numberOfInsertedTiles = 0;

    size_t z = 0;
    while(z < m_tiles.size() && numberOfInsertedTiles < count)
    {
        numberOfInsertedTiles += insert(tile, z, count - numberOfInsertedTiles);
    }

    return numberOfInsertedTiles;
}
int TileStack::insert(Tile* tile, size_t slotId, int count)
{
    if(slotId >= m_tiles.size()) return 0;

    if(count == -1) count = std::numeric_limits<int>::max();
    if(count <= 0) return 0;

    Tile* storedTile = m_tiles[slotId];
    if(storedTile == nullptr)
    {
        storedTile = tile->clone().release();
        storedTile->setQuantity(0); //after inserting it won't be 0
    }
    else if(!storedTile->equals(*tile))
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

int TileStack::erase(Tile* tile, int count)
{
    if(count == -1) count = std::numeric_limits<int>::max();
    int numberOfErasedTiles = 0;

    size_t z = 0;
    while(z < m_tiles.size() && numberOfErasedTiles < count)
    {
        numberOfErasedTiles += erase(tile, z, count - numberOfErasedTiles);
    }
    
    return numberOfErasedTiles;
}
int TileStack::erase(Tile* tile, size_t slotId, int count)
{
    if(slotId >= m_tiles.size()) return 0;

    Tile* storedTile = m_tiles[slotId];
    if(storedTile == nullptr) return 0;
    if(!storedTile->equals(*tile)) return 0;
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

const std::vector<Tile*>& TileStack::tiles() const
{
    return m_tiles;
}

bool TileStack::isValid(int z) const
{
    return m_lastTile >= z;
}

const Tile& TileStack::at(int z) const
{
    if(isValid(z)) return *(m_tiles[z]);
    return m_emptyTile;
}
Tile& TileStack::at(int z)
{
    if(isValid(z)) return *(m_tiles[z]);
    return m_emptyTile;
}
int TileStack::size() const
{
    return m_lastTile + 1;
}

bool TileStack::isTall() const
{
    for(const Tile* tile : m_tiles)
        if(tile->view().isTall()) return true;

    return false;
}

bool TileStack::hasCollider() const
{
    for(const Tile* tile : m_tiles)
    {
        if(tile->model().hasCollider()) return true;
    }

    return false;
}
const RectangleF& TileStack::collider() const
{
    for(const Tile* tile : m_tiles)
    {
        if(tile->model().hasCollider()) return tile->model().collider();
    }

    return m_defaultCollider;
}

int TileStack::topZ() const
{
    return m_lastTile;
}
