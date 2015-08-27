#include "InventorySystem.h"

#include "AttemptToInteractWithExternalInventory.h"
#include "PlayerEquipmentInventory.h"

#include <algorithm>

InventorySystem::InventorySystem() :
    m_equipmentInventory()
{
    m_trackedInventories.push_back(TrackedInventory::createInternal(&m_equipmentInventory, nullptr));

    EventDispatcher::instance().subscribe<AttemptToInteractWithExternalInventory>(std::bind(&InventorySystem::onAttemptToInteractWithExternalInventory, this, std::placeholders::_1));
}

bool InventorySystem::tryOpenExternalInventory(Inventory* inventory, int x, int y)
{
    TrackedInventory* candidateToRecycle = findInventory(inventory);
    if(candidateToRecycle == nullptr)
    {
        m_trackedInventories.push_back(TrackedInventory::createExternal(inventory, x, y));
    }
    else
    {
        return tryReopenTrackedInventory(*candidateToRecycle);
    }

    return true;
}
bool InventorySystem::tryOpenInternalInventory(Inventory* inventory, TrackedInventory& parentInventory)
{
    TrackedInventory* candidateToRecycle = findInventory(inventory);
    if(candidateToRecycle == nullptr)
    {
        m_trackedInventories.push_back(TrackedInventory::createInternal(inventory, &parentInventory));
    }
    else
    {
        return tryReopenTrackedInventory(*candidateToRecycle);
    }

    return true;
}
bool InventorySystem::tryReopenTrackedInventory(TrackedInventory& inventory)
{
    inventory.isOpened = true;
    moveInventoryToTheEnd(inventory);
    return true;
}
void InventorySystem::moveInventoryToTheEnd(TrackedInventory& inventory)
{
    auto iter = std::find(m_trackedInventories.begin(), m_trackedInventories.end(), inventory);
    std::rotate(iter, iter + 1, m_trackedInventories.end());
}
void InventorySystem::closeInventory(Inventory* inventory) //assumes it is present
{
    TrackedInventory* foundInventory = findInventory(inventory);
    foundInventory->isOpened = false;
    if(!isParentOfAnyInventory(*foundInventory)) //is the last opened inventory in the chain. Should be removed along with all closed parents
    {
        TrackedInventory* current = foundInventory;
        while(current->isOpened == false)
        {
            TrackedInventory* parent = current->parentInventory;
            abandonInventory(*current);
            if(parent == nullptr || isParentOfAnyInventory(*parent)) break; //if it still a parent of some inventory we can't delete it

            current = parent;
        }
    }
}
void InventorySystem::abandonInventory(TrackedInventory& inventory)
{
    m_trackedInventories.erase(std::remove(m_trackedInventories.begin(), m_trackedInventories.end(), inventory), m_trackedInventories.end());
}
bool InventorySystem::isInventoryOpened(Inventory* inventory)
{
    TrackedInventory* candidate = findInventory(inventory);
    if(candidate == nullptr) return false;
    else return candidate->isOpened;
}
bool InventorySystem::isInventoryTracked(Inventory* inventory)
{
    return findInventory(inventory) != nullptr;
}
bool InventorySystem::isParentOfAnyInventory(TrackedInventory& inventory)
{
    return std::find_if(m_trackedInventories.begin(), m_trackedInventories.end(), [inventory](const TrackedInventory& inv){return *(inv.parentInventory) == inventory;}) != m_trackedInventories.end();
}

void InventorySystem::onAttemptToInteractWithExternalInventory(const AttemptToInteractWithExternalInventory& event)
{
    tryOpenExternalInventory(event.inventory(), event.x(), event.y());
}
InventorySystem::TrackedInventory* InventorySystem::findInventory(Inventory* inventory)
{
    auto iter = std::find(m_trackedInventories.begin(), m_trackedInventories.end(), inventory);

    if(iter == m_trackedInventories.end()) return nullptr;
    else return &(*iter);
}

PlayerEquipmentInventory& InventorySystem::equipmentInventory()
{
    return m_equipmentInventory;
}
std::vector<InventorySystem::TrackedInventory*> InventorySystem::openedInventories()
{
    std::vector<TrackedInventory*> result;
    for(auto& inv : m_trackedInventories)
    {
        if(inv.isOpened)
        {
            result.push_back(&inv);
        }
    }
    return result;
}
