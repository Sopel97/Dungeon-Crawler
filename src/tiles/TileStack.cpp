#include "..\..\include\tiles\TileStack.h"

TileStack::TileStack() :
    m_tile(nullptr),
    m_quantity(0)
{

}
TileStack::TileStack(std::unique_ptr<Tile>&& tile, int quantity) :
    m_tile(std::move(tile)),
    m_quantity(quantity)
{
    if (m_quantity != 1) m_tile->onTileQuantityChanged(1, quantity);
}

bool TileStack::isEmpty() const
{
    return m_tile == nullptr;
}

int TileStack::quantity() const
{
    return m_quantity;
}
int TileStack::maxQuantity() const
{
    return m_tile->maxQuantity();
}
void TileStack::setQuantity(int newQuantity)
{
    if (newQuantity < 1)
    {
        m_quantity = 0;
        m_tile = nullptr;
        return;
    }
    if (newQuantity > m_tile->maxQuantity()) newQuantity = m_tile->maxQuantity();

    if (m_quantity != newQuantity)
    {
        m_quantity = newQuantity;

        m_tile->onTileQuantityChanged(m_quantity, newQuantity);
    }
}
void TileStack::insert(int delta)
{
    setQuantity(m_quantity + delta);
}
void TileStack::erase(int delta)
{
    setQuantity(m_quantity - delta);
}
int TileStack::spaceLeft() const
{
    return maxQuantity() - m_quantity;
}
Tile& TileStack::tile()
{
    return *m_tile.get();
}
const Tile& TileStack::tile() const
{
    return *m_tile.get();
}
TileStack TileStack::split(int newStackSize)
{
    TileStack newStack = clone();
    newStack.setQuantity(newStackSize);
    setQuantity(m_quantity - newStackSize);

    return std::move(newStack);
}
TileStack TileStack::clone() const
{
    return TileStack(m_tile->clone(), m_quantity);
}
