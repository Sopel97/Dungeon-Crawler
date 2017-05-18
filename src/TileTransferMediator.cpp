#include "TileTransferMediator.h"

#include "Player.h"
#include "Inventory.h"
#include "InventorySystem.h"

TileTransferMediator::TileTransferMediator(InventorySystem& inventorySystem, Player& player) :
    m_inventorySystem(&inventorySystem),
    m_player(&player),
    m_source(std::nullopt)
{

}
void TileTransferMediator::grab(const FromWorld& from)
{
    m_source.emplace(from);
}
void TileTransferMediator::grab(const FromInventory& from)
{
    m_source.emplace(from);
}
void TileTransferMediator::put(const ToWorld& to)
{
    if (!m_source.has_value()) return;

    std::variant<ToWorld, ToInventory> destination(to);
    std::visit(*this, m_source.value(), destination);

    m_source.reset();
}
void TileTransferMediator::put(const ToInventory& to)
{
    if (!m_source.has_value()) return;

    std::variant<ToWorld, ToInventory> destination(to);
    std::visit(*this, m_source.value(), destination);

    m_source.reset();
}

bool TileTransferMediator::isAnyTileGrabbed() const
{
    return m_source.has_value();
}
void TileTransferMediator::operator()(const FromWorld& from, const ToWorld& to)
{

}
void TileTransferMediator::operator()(const FromWorld& from, const ToInventory& to)
{

}
void TileTransferMediator::operator()(const FromInventory& from, const ToWorld& to)
{

}
void TileTransferMediator::operator()(const FromInventory& from, const ToInventory& to)
{

}