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
    tryOpenInternalInventory(&m_equipmentInventory, nullptr);
}

bool InventorySystem::tryOpenExternalInventory(Inventory* inventory, int x, int y)
{
    TrackedInventory* trackedInventory = findTrackedInventory(inventory);
    if(trackedInventory == nullptr)
    {
        trackedInventory = &(*m_trackedInventories.emplace(m_trackedInventories.end(), TrackedInventory::createExternal(inventory, x, y)));
    }

    openInventory(trackedInventory);

    return true;
}
bool InventorySystem::tryOpenInternalInventory(Inventory* inventory, TrackedInventory* parentInventory)
{
    TrackedInventory* trackedInventory = findTrackedInventory(inventory);
    if(trackedInventory == nullptr)
    {
        trackedInventory = &(*m_trackedInventories.emplace(m_trackedInventories.end(), TrackedInventory::createInternal(inventory, parentInventory)));
    }

    openInventory(trackedInventory);

    return true;
}
void InventorySystem::closeInventory(Inventory* inventory)
{
    TrackedInventory* openedInventory = findTrackedInventory(inventory);
    if(openedInventory == nullptr) return;
    m_openedInventories.remove(openedInventory);
    m_playerUi.closeWindow(&(openedInventory->inventoryView));

    openedInventory->isOpened = false;
    TrackedInventory* current = openedInventory;
    while(current != nullptr && current->isOpened == false)
    {   
        if(isParentOfAnyInventory(*current)) break;

        TrackedInventory* parent = current->parentInventory;
        abandonInventory(*current);
        current = parent;
    }

    updatePositionsOfOpenedInventories();
}
void InventorySystem::abandonInventory(TrackedInventory& inventory)
{
    m_trackedInventories.remove(inventory);
}
bool InventorySystem::isInventoryOpened(Inventory* inventory)
{
    return std::find_if(m_openedInventories.begin(), m_openedInventories.end(), [inventory](const TrackedInventory * inv) {return inv->inventory == inventory;}) != m_openedInventories.end();
}
bool InventorySystem::isInventoryTracked(Inventory* inventory)
{
    return findTrackedInventory(inventory) != nullptr;
}
bool InventorySystem::isParentOfAnyInventory(TrackedInventory& inventory)
{
    return std::find_if(m_trackedInventories.begin(), m_trackedInventories.end(), [inventory](const TrackedInventory & inv) {return inv.parentInventory == &inventory;}) != m_trackedInventories.end();
}

void InventorySystem::updatePositionsOfOpenedInventories()
{
    int currentHeight = 0;
    for(auto& inventory : m_openedInventories)
    {
        InventoryView& inventoryView = inventory->inventoryView;
        inventoryView.setOffsetFromTop(currentHeight);
        currentHeight += inventoryView.windowHeight();
    }
}

bool InventorySystem::tryInteractWithExternalInventory(Inventory* inventory, const TileLocation& location)
{
    if (isInventoryOpened(inventory))
    {
        closeInventory(inventory);
        return true;
    }
    else
    {
        return tryOpenExternalInventory(inventory, location.x, location.y);
    }
}

InventorySystem::TrackedInventory* InventorySystem::findTrackedInventory(Inventory* inventory)
{
    auto iter = std::find(m_trackedInventories.begin(), m_trackedInventories.end(), inventory);

    if(iter == m_trackedInventories.end()) return nullptr;
    else return &(*iter);
}

PlayerEquipmentInventory& InventorySystem::equipmentInventory()
{
    return m_equipmentInventory;
}
const std::list<InventorySystem::TrackedInventory*>& InventorySystem::openedInventories() const
{
    return m_openedInventories;
}

void InventorySystem::openInventory(TrackedInventory* inventory)
{
    m_openedInventories.push_back(inventory);
    m_playerUi.openPanelWindow(&(inventory->inventoryView));
    inventory->isOpened = true;
    inventory->inventoryView.setContentHeightToMax();

    updatePositionsOfOpenedInventories();
}