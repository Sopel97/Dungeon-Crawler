#include "InventorySystem.h"

#include "PlayerEquipmentInventory.h"

#include "Player.h"
#include "TileLocation.h"

#include <algorithm>

InventorySystem::InventorySystem(Player& player) :
    m_player(player),
    m_playerUi(player.playerUi()),
    m_equipmentInventory()
{
    openPermanentInventory(m_equipmentInventory);
}

bool InventorySystem::tryOpenExternalInventory(Inventory& inventory, const ls::Vec2I& pos)
{
    TrackedInventoryHandle h = find(inventory).second;
    if(!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makeExternal(inventory, pos));
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
        h = tree.emplaceChild(trackedParentHandle, TrackedInventory::makeInternal(inventory));
    }

    openTrackedInventory(h);

    return true;
}
void InventorySystem::openPermanentInventory(Inventory& inventory)
{
    TrackedInventoryHandle h = find(inventory).second;
    if (!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makePermanent(inventory));
        h = treeIter->root();
    }

    openTrackedInventory(h);
}
void InventorySystem::closeInventory(Inventory& inventory)
{
    auto findResult = find(inventory);
    TrackedInventoryTreeHandle treeHandle = findResult.first;
    TrackedInventoryHandle inventoryHandle = findResult.second;
    if (!inventoryHandle.isValid() || !inventoryHandle.data().isOpened) return; //nothing to do

    m_playerUi.closeWindow(inventoryHandle.data().inventoryWindow.get());
    inventoryHandle.data().isOpened = false;
    if (inventoryHandle.hasParent()) return; // we can't abandon it since some inventories depent on it

    TrackedInventoryHandle current = inventoryHandle;
    while(current.isValid() && !current.data().isOpened) //abandon all inventories that were just because this one was opened
    {
        auto parent = current.parent(); //we have to remember it here since abandon inventory deletes current
        abandonInventory(treeHandle, current);
        current = parent;
    }
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
bool InventorySystem::isInventoryOpened(Inventory& inventory)
{
    auto h = find(inventory).second;
    return h.isValid() && h.data().isOpened;
}
bool InventorySystem::isInventoryTracked(Inventory& inventory)
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

std::pair<InventorySystem::TrackedInventoryTreeHandle, InventorySystem::TrackedInventoryHandle> InventorySystem::find(Inventory& inventory)
{
    return m_trackedInventories.findIf([&inventory](const TrackedInventory& inv) {return inv.inventory == &inventory; });
}

PlayerEquipmentInventory& InventorySystem::equipmentInventory()
{
    return m_equipmentInventory;
}

void InventorySystem::openTrackedInventory(TrackedInventoryHandle inventory)
{
	inventory.data().inventoryWindow->setContentHeightToMax();
	inventory.data().isOpened = true;

    m_playerUi.openWindow(inventory.data().inventoryWindow.get());
}