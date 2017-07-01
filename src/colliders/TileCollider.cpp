#include "colliders/TileCollider.h"

TileCollider::TileCollider(Tile& tile, const ls::Rectangle2F& volume) :
    m_parent(&tile),
    m_volume(volume)
{

}

const ls::Rectangle2F& TileCollider::volume() const
{
    return m_volume;
}
ls::Rectangle2F TileCollider::boundingBox() const
{
    return m_volume;
}
const Tile& TileCollider::tile() const
{
    return *m_parent;
}
Tile& TileCollider::tile()
{
    return *m_parent;
}