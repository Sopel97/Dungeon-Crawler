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
    TrackedInventoryHandle h = find(inventory);
    if(!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makeExternal(inventory, pos));
        h = m_trackedInventories.tree(treeIter).root();
    }

    openTrackedInventory(h);

    return true;
}
bool InventorySystem::tryOpenInternalInventory(Inventory& inventory, Inventory& parentInventory)
{
    TrackedInventoryHandle trackedParentHandle = find(parentInventory);
    if (!trackedParentHandle.isValid() || !trackedParentHandle.data().isOpened) return false;

    TrackedInventoryHandle h = find(inventory);
    if (!h.isValid())
    {
        auto& tree = trackedParentHandle.tree();
        h = tree.insertChild(trackedParentHandle, TrackedInventory::makeInternal(inventory));
    }

    openTrackedInventory(h);

    return true;
}
void InventorySystem::openPermanentInventory(Inventory& inventory)
{
    TrackedInventoryHandle h = find(inventory);
    if (!h.isValid())
    {
        auto treeIter = m_trackedInventories.emplaceTree(TrackedInventory::makePermanent(inventory));
        h = m_trackedInventories.tree(treeIter).root();
    }

    openTrackedInventory(h);
}
void InventorySystem::closeInventory(Inventory& inventory)
{
    TrackedInventoryHandle inventoryHandle = find(inventory);
    if (!inventoryHandle.isValid() || !inventoryHandle.data().isOpened) return; //nothing to do

    m_playerUi.closeWindow(inventoryHandle.data().inventoryWindow.get());
    inventoryHandle.data().isOpened = false;
    if (inventoryHandle.hasParent()) return; // we can't abandon it since some inventories depent on it

    TrackedInventoryHandle current = inventoryHandle;
    while(current.isValid() && !current.data().isOpened) //abandon all inventories that were just because this one was opened
    {
        auto parent = current.parent(); //we have to remember it here since abandon inventory deletes current
        abandonInventory(current);
        current = parent;
    }
}
void InventorySystem::abandonInventory(TrackedInventoryHandle inventory)
{
    // has no children

    // is not root
    if (inventory.hasParent())
    {
        inventory.tree().remove(inventory);
    }
    else //if root then delete whole tree
    {
        auto treeIter = m_trackedInventories.findTree(&(inventory.tree()));
        m_trackedInventories.removeTree(treeIter);
    }
}
bool InventorySystem::isInventoryOpened(Inventory& inventory)
{
    auto h = find(inventory);
    return h.isValid() && h.data().isOpened;
}
bool InventorySystem::isInventoryTracked(Inventory& inventory)
{
    return find(inventory).isValid();
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

InventorySystem::TrackedInventoryHandle InventorySystem::find(Inventory& inventory)
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