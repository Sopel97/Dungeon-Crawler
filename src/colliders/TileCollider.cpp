#include "colliders/TileCollider.h"

TileCollider::TileCollider(Tile& tile, const ls::Rectangle2F& volume) :
    m_parent(&tile),
    m_volume(volume)
{

}

const ls::Rectangle2F& TileCollider::volume()
{
    return m_volume;
}
ls::Rectangle2F TileCollider::boundingBox()
{
    return m_volume;
}