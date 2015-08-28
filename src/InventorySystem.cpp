#include "InventorySystem.h"

#include "AttemptToInteractWithExternalInventory.h"
#include "PlayerEquipmentInventory.h"

#include <algorithm>

InventorySystem::InventorySystem() :
    m_equipmentInventory()
{
    tryOpenInternalInventory(&m_equipmentInventory, nullptr);

    EventDispatcher::instance().subscribe<AttemptToInteractWithExternalInventory>(std::bind(&InventorySystem::onAttemptToInteractWithExternalInventory, this, std::placeholders::_1));
}

bool InventorySystem::tryOpenExternalInventory(Inventory* inventory, int x, int y)
{
    TrackedInventory* trackedInventory = findTrackedInventory(inventory);
    if(trackedInventory == nullptr)
    {
        trackedInventory = &(*m_trackedInventories.emplace(m_trackedInventories.end(), TrackedInventory::createExternal(inventory, x, y)));
    }

    m_openedInventories.push_back(trackedInventory);
    trackedInventory->isOpened = true;

    updatePositionsOfOpenedInventories();

    return true;
}
bool InventorySystem::tryOpenInternalInventory(Inventory* inventory, TrackedInventory* parentInventory)
{
    TrackedInventory* trackedInventory = findTrackedInventory(inventory);
    if(trackedInventory == nullptr)
    {
        trackedInventory = &(*m_trackedInventories.emplace(m_trackedInventories.end(), TrackedInventory::createInternal(inventory, parentInventory)));
    }

    m_openedInventories.push_back(trackedInventory);
    trackedInventory->isOpened = true;

    updatePositionsOfOpenedInventories();

    return true;
}
void InventorySystem::closeInventory(Inventory* inventory)
{
    TrackedInventory* openedInventory = findTrackedInventory(inventory);
    if(openedInventory == nullptr) return;
    m_openedInventories.erase(std::remove(m_openedInventories.begin(), m_openedInventories.end(), openedInventory), m_openedInventories.end());

    openedInventory->isOpened = false;
    TrackedInventory* current = openedInventory;
    while(current != nullptr && current->isOpened == false && !isParentOfAnyInventory(*current))
    {
        TrackedInventory* parent = current->parentInventory;
        abandonInventory(*current);
        current = parent;
    }

    updatePositionsOfOpenedInventories();
}
void InventorySystem::abandonInventory(TrackedInventory& inventory)
{
    m_trackedInventories.erase(std::remove(m_trackedInventories.begin(), m_trackedInventories.end(), inventory), m_trackedInventories.end());
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
    return std::find_if(m_trackedInventories.begin(), m_trackedInventories.end(), [inventory](const TrackedInventory & inv) {return *(inv.parentInventory) == inventory;}) != m_trackedInventories.end();
}

void InventorySystem::updatePositionsOfOpenedInventories()
{
    int currentHeight = 0;
    for(auto& inventory : m_openedInventories)
    {
        InventoryView& inventoryView = inventory->inventoryView;;
        inventoryView.setOffsetFromTop(currentHeight);
        currentHeight += inventoryView.height();
    }
}

void InventorySystem::onAttemptToInteractWithExternalInventory(const AttemptToInteractWithExternalInventory& event)
{
    tryOpenExternalInventory(event.inventory(), event.x(), event.y());
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
const std::vector<InventorySystem::TrackedInventory*>& InventorySystem::openedInventories() const
{
    return m_openedInventories;
}