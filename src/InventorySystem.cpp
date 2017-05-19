#include "InventorySystem.h"

#include "PlayerEquipmentInventory.h"

#include "Player.h"
#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "TileLocation.h"
#include "tiles/Tile.h"
#include "tiles/TileStack.h"
#include "tiles/models/TileModel.h"

#include "events/TileMovedFromWorldToWorld.h"
#include "events/TileMovedFromWorldToInventory.h"

#include "TileTransferMediator.h"

#include <algorithm>

InventorySystem::InventorySystem(WindowSpaceManager& wsm, Player& player, TileTransferMediator& tileTransferMediator) :
    m_wsm(wsm),
    m_player(player),
    m_playerUi(player.playerUi()),
    m_tileTransferMediator(tileTransferMediator),
    m_equipmentInventory(),
    m_tileMovedFromWorldToWorldEventSubscription(EventDispatcher::instance().subscribe<TileMovedFromWorldToWorld>([this](const TileMovedFromWorldToWorld& e) {onTileMovedFromWorldToWorld(e); })),
    m_tileMovedFromWorldToInventoryEventSubscription(EventDispatcher::instance().subscribe<TileMovedFromWorldToInventory>([this](const TileMovedFromWorldToInventory& e) {onTileMovedFromWorldToInventory(e); }))
{
    openPermanentInventory(m_equipmentInventory);
}

bool InventorySystem::tryOpenExternalInventory(Inventory& inventory, const ls::Vec2I& pos)
{
    TrackedInventoryHandle h = find(inventory).second;
    if(!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makeExternal(m_wsm, *this, inventory, pos));
        h = treeIter->root();
    }

    openTrackedInventory(h);

    return true;
}
bool InventorySystem::tryOpenInternalInventory(Inventory& inventory, Inventory& parentInventory)
{
    auto findResult = find(parentInventory);
    TrackedInventoryTreeHandle treeHandle = findResult.first;
    TrackedInventoryHandle trackedParentHandle = findResult.second;
    if (!trackedParentHandle.isValid() || !trackedParentHandle.data().isOpened) return false;

    TrackedInventoryHandle h = find(inventory).second;
    if (!h.isValid())
    {
        auto& tree = *treeHandle;
        h = tree.emplaceChild(trackedParentHandle, TrackedInventory::makeInternal(m_wsm, *this, inventory));
    }

    openTrackedInventory(h);

    return true;
}
void InventorySystem::openPermanentInventory(Inventory& inventory)
{
    TrackedInventoryHandle h = find(inventory).second;
    if (!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makePermanent(m_wsm, *this, inventory));
        h = treeIter->root();
    }

    openTrackedInventory(h);
}
void InventorySystem::closeInventory(Inventory& inventory)
{
    closeInventory(find(inventory));
}
void InventorySystem::abandonInventory(TrackedInventoryTreeHandle tree, TrackedInventoryHandle inventory)
{
    // has no children

    // is not root
    if (inventory.hasParent())
    {
        tree->remove(inventory);
    }
    else //if root then delete whole tree
    {
        m_trackedInventories.removeTree(tree);
    }
}
bool InventorySystem::isInventoryOpened(const Inventory& inventory)
{
    auto h = find(inventory).second;
    return h.isValid() && h.data().isOpened;
}
bool InventorySystem::isInventoryTracked(const Inventory& inventory)
{
    return find(inventory).second.isValid();
}

bool InventorySystem::tryInteractWithExternalInventory(Inventory& inventory, const TileLocation& location)
{
    if (isInventoryOpened(inventory))
    {
        closeInventory(inventory);
        return true;
    }
    else
    {
        return tryOpenExternalInventory(inventory, ls::Vec2I(location.x, location.y));
    }
}

bool InventorySystem::canStore(const Inventory& inventory, const Tile& tile) const
{
    if (!tile.model().canBeStored()) return false;

    const Inventory* tileInventory = tile.model().inventory();
    if (tileInventory == nullptr) return true;

    ConstTrackedInventoryHandle current = find(inventory).second;
    if (!current.isValid()) return false; // inventory is not tracked

    for (;;)
    {
        if (current.data().inventory == tileInventory) return false; // cycle found
        if (current.hasParent()) current = current.parent();
        else break;
    }

    return true; // no cycle found
}

std::pair<InventorySystem::TrackedInventoryTreeHandle, InventorySystem::TrackedInventoryHandle> InventorySystem::find(const Inventory& inventory)
{
    return m_trackedInventories.findIf([&inventory](const TrackedInventory& inv) {return inv.inventory == &inventory; });
}
std::pair<InventorySystem::ConstTrackedInventoryTreeHandle, InventorySystem::ConstTrackedInventoryHandle> InventorySystem::find(const Inventory& inventory) const
{
    return m_trackedInventories.findIf([&inventory](const TrackedInventory& inv) {return inv.inventory == &inventory; });
}

PlayerEquipmentInventory& InventorySystem::equipmentInventory()
{
    return m_equipmentInventory;
}
TileTransferMediator& InventorySystem::tileTransferMediator()
{
    return m_tileTransferMediator;
}
void InventorySystem::onTileMovedFromWorldToWorld(const TileMovedFromWorldToWorld& event)
{
    const Tile& tile = event.tileAfterMove->tile();
    const Inventory* inv = tile.model().inventory();
    if (inv == nullptr) return;

    auto trackedInvHandle = find(*inv).second;
    if (!trackedInvHandle.isValid()) return;

    auto& trackedInv = trackedInvHandle.data();
    if (trackedInv.inventory == inv)
    {
        if (!trackedInv.worldPosition.has_value()) return;

        trackedInv.worldPosition = ls::Vec2I(event.destination.pos.x, event.destination.pos.y);
    }
}
void InventorySystem::onTileMovedFromWorldToInventory(const TileMovedFromWorldToInventory& event)
{
    const Tile& tile = event.tileAfterMove->tile();
    const Inventory* tileInventory = tile.model().inventory();
    if (tileInventory == nullptr) return;

    auto foundTileInventory = find(*tileInventory);
    auto tileInventoryHandle = foundTileInventory.second;
    if (!tileInventoryHandle.isValid()) return;

    tileInventoryHandle.data().worldPosition.reset(); // no longer in the world
    auto tileInventoryTreeHandle = foundTileInventory.first;
    auto detachedTileInventoryTree = tileInventoryTreeHandle->detach(tileInventoryHandle);
    m_trackedInventories.removeTree(tileInventoryTreeHandle); // if we moved from world then it had to be the root

    auto foundDestinationInventory = find(*event.destination.inventory);
    auto destinationTreeHandle = foundDestinationInventory.first;
    auto destinationInventoryHandle = foundDestinationInventory.second;
    destinationTreeHandle->attach(destinationInventoryHandle, std::move(detachedTileInventoryTree));
}

void InventorySystem::openTrackedInventory(TrackedInventoryHandle inventory)
{
	inventory.data().inventoryWindow->setContentHeightToMax();
	inventory.data().isOpened = true;

    m_playerUi.openWindow(inventory.data().inventoryWindow.get());
}

std::pair<InventorySystem::TrackedInventoryTreeHandle, InventorySystem::TrackedInventoryHandle> InventorySystem::find(const InternalWindow& wnd)
{
    return m_trackedInventories.findIf([&wnd](const TrackedInventory& inv) {return &wnd == inv.inventoryWindow.get(); });
}

void InventorySystem::closeInventory(const std::pair<TrackedInventoryTreeHandle, TrackedInventoryHandle>& findResult)
{
    TrackedInventoryTreeHandle treeHandle = findResult.first;
    TrackedInventoryHandle inventoryHandle = findResult.second;
    if (!inventoryHandle.isValid() || !inventoryHandle.data().isOpened) return; //nothing to do

    m_playerUi.closeWindow(inventoryHandle.data().inventoryWindow.get());
    inventoryHandle.data().isOpened = false;
    if (inventoryHandle.numberOfChildren() > 0) return; // we can't abandon it since some inventories depent on it

    TrackedInventoryHandle current = inventoryHandle;
    while (current.isValid() && !current.data().isOpened) //abandon all inventories that were just because this one was opened
    {
        auto parent = current.parent(); //we have to remember it here since abandon inventory deletes current
        abandonInventory(treeHandle, current);
        current = parent;
    }
}
void InventorySystem::closeInventory(InternalWindow& wnd)
{
    closeInventory(find(wnd));
}