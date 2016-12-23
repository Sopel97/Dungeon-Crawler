#include "..\..\include\tiles\TileStack.h"

TileStack::TileStack(std::unique_ptr<Tile>&& tile, int quantity) :
    m_tile(std::move(tile)),
    m_quantity(quantity)
{

}

int TileStack::quantity() const
{
    return m_quantity;
}
void TileStack::setQuantity(int newQuantity)
{
    if (newQuantity < 1) newQuantity = 1;
    if (newQuantity > m_tile->maxQuantity()) newQuantity = m_tile->maxQuantity();

    if (m_quantity != newQuantity)
    {
        m_quantity = newQuantity;

        m_tile->onTileQuantityChanged(m_quantity, newQuantity);

        m_quantity = newQuantity;
    }
}
int TileStack::maxQuantity() const
{
    return m_tile->maxQuantity();
}
Tile* TileStack::tile()
{
    return m_tile.get();
}
const Tile* TileStack::tile() const
{
    return m_tile.get();
}
std::unique_ptr<TileStack> TileStack::clone() const
{
    return std::make_unique<TileStack>(m_tile->clone(), m_quantity);
}
