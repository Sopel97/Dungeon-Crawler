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
#include "events/TileMovedFromInventoryToInventory.h"
#include "events/TileMovedFromInventoryToWorld.h"

#include "TileTransferMediator.h"

#include <algorithm>

InventorySystem::InventorySystem(WindowSpaceManager& wsm, Player& player, TileTransferMediator& tileTransferMediator) :
    m_wsm(wsm),
    m_player(player),
    m_playerUi(player.playerUi()),
    m_tileTransferMediator(tileTransferMediator),
    m_equipmentInventory(),
    m_tileMovedFromWorldToWorldEventSubscription(EventDispatcher::instance().subscribe<TileMovedFromWorldToWorld>([this](const TileMovedFromWorldToWorld& e) {onTileMovedFromWorldToWorld(e); })),
    m_tileMovedFromWorldToInventoryEventSubscription(EventDispatcher::instance().subscribe<TileMovedFromWorldToInventory>([this](const TileMovedFromWorldToInventory& e) {onTileMovedFromWorldToInventory(e); })),
    m_tileMovedFromInventoryToInventoryEventSubscription(EventDispatcher::instance().subscribe<TileMovedFromInventoryToInventory>([this](const TileMovedFromInventoryToInventory& e) {onTileMovedFromInventoryToInventory(e); })),
    m_tileMovedFromInventoryToWorldEventSubscription(EventDispatcher::instance().subscribe<TileMovedFromInventoryToWorld>([this](const TileMovedFromInventoryToWorld& e) {onTileMovedFromInventoryToWorld(e); }))
{
    openPermanentInventory(m_equipmentInventory, "");
}

bool InventorySystem::tryOpenExternalInventory(Tile& tile, Inventory& inventory, const ls::Vec2I& pos)
{
    TrackedInventoryHandle h = find(inventory).second;
    if(!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makeExternal(m_wsm, *this, inventory, tile.model().displayedName(), pos));
        h = treeIter->root();
    }

    openTrackedInventory(h);

    return true;
}
bool InventorySystem::tryOpenInternalInventory(Tile& tile, Inventory& inventory, const Inventory& parentInventory)
{
    auto findResult = find(parentInventory);
    TrackedInventoryTreeHandle treeHandle = findResult.first;
    TrackedInventoryHandle trackedParentHandle = findResult.second;
    if (!trackedParentHandle.isValid() || !trackedParentHandle.data().isOpened) return false;

    TrackedInventoryHandle h = find(inventory).second;
    if (!h.isValid())
    {
        auto& tree = *treeHandle;
        h = tree.emplaceChild(trackedParentHandle, TrackedInventory::makeInternal(m_wsm, *this, inventory, tile.model().displayedName()));
    }

    openTrackedInventory(h);

    return true;
}
void InventorySystem::openPermanentInventory(Inventory& inventory, const std::string& name)
{
    TrackedInventoryHandle h = find(inventory).second;
    if (!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makePermanent(m_wsm, *this, inventory, name));
        h = treeIter->root();
    }

    openTrackedInventory(h);
}
void InventorySystem::closeInventory(Inventory& inventory)
{
    closeInventory(find(inventory).second);
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

bool InventorySystem::tryInteractWithExternalInventory(Tile& tile, Inventory& inventory, const TileLocation& location)
{
    if (isInventoryOpened(inventory))
    {
        closeInventory(inventory);
        return true;
    }
    else
    {
        return tryOpenExternalInventory(tile, inventory, ls::Vec2I(location.x, location.y));
    }
}
bool InventorySystem::tryInteractWithInternalInventory(Tile& tile, Inventory& inventory, const InventorySlotView& slot)
{
    if (isInventoryOpened(inventory))
    {
        closeInventory(inventory);
        return true;
    }
    else
    {
        return tryOpenInternalInventory(tile, inventory, slot.inventory());
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
Player& InventorySystem::player()
{
    return m_player;
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
void InventorySystem::onTileMovedFromInventoryToInventory(const TileMovedFromInventoryToInventory& event)
{
    if (event.source.inventory == event.destination.inventory) return;

    const Tile& tile = event.tileAfterMove->tile();
    const Inventory* tileInventory = tile.model().inventory();
    if (tileInventory == nullptr) return;

    auto foundTileInventory = find(*tileInventory);
    auto tileInventoryHandle = foundTileInventory.second;
    if (!tileInventoryHandle.isValid()) return;

    auto tileInventoryTreeHandle = foundTileInventory.first;
    auto detachedTileInventoryTree = tileInventoryTreeHandle->detach(tileInventoryHandle);

    auto foundDestinationInventory = find(*event.destination.inventory);
    auto destinationTreeHandle = foundDestinationInventory.first;
    auto destinationInventoryHandle = foundDestinationInventory.second;
    destinationTreeHandle->attach(destinationInventoryHandle, std::move(detachedTileInventoryTree));
}
void InventorySystem::onTileMovedFromInventoryToWorld(const TileMovedFromInventoryToWorld& event)
{
    const Tile& tile = event.tileAfterMove->tile();
    const Inventory* tileInventory = tile.model().inventory();
    if (tileInventory == nullptr) return;

    auto foundTileInventory = find(*tileInventory);
    auto tileInventoryHandle = foundTileInventory.second;
    if (!tileInventoryHandle.isValid()) return;

    tileInventoryHandle.data().worldPosition = ls::Vec2I(event.destination.pos.x, event.destination.pos.y);
    auto tileInventoryTreeHandle = foundTileInventory.first;
    auto detachedTileInventoryTree = tileInventoryTreeHandle->detach(tileInventoryHandle);

    m_trackedInventories.emplaceTree(std::move(detachedTileInventoryTree));
}
void InventorySystem::onInventoryWindowClosed(Inventory& inventory)
{
    auto findResult = find(inventory);
    TrackedInventoryTreeHandle treeHandle = findResult.first;
    TrackedInventoryHandle inventoryHandle = findResult.second;

    inventoryHandle.data().resetWindow();

    TrackedInventoryHandle current = inventoryHandle;
    while (current.isValid() && current.numberOfChildren() == 0 && !current.data().isOpened) //abandon all inventories that were just because this one was opened
    {
        auto parent = current.parent(); //we have to remember it here since abandon inventory deletes current
        abandonInventory(treeHandle, current);
        current = parent;
    }
}

void InventorySystem::openTrackedInventory(TrackedInventoryHandle inventory)
{
    auto inventoryWindow = inventory.data().createInventoryWindow(m_wsm);
    inventoryWindow->updateMaxContentHeight(*((inventory.data()).inventoryView));
    inventoryWindow->setContentHeightToMax();
    inventoryWindow->attachContent(*((inventory.data()).inventoryView));

    inventory.data().setWindow(inventoryWindow.get());

    m_playerUi.openWindow(std::move(inventoryWindow));
}

void InventorySystem::closeInventory(TrackedInventoryHandle& inventoryHandle)
{
    if (!inventoryHandle.isValid() || !inventoryHandle.data().isOpened) return; //nothing to do

    m_playerUi.closeWindow(inventoryHandle.data().inventoryWindow); // eventually invokes onInventoryWindowClosed(...)
}