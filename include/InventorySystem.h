#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include "InventoryView.h"
#include "PlayerEquipmentInventory.h"

class Inventory;
class AttemptToInteractWithExternalInventory;

class InventorySystem
{
public:
    struct TrackedInventory //forms a forward linked list
    {
        enum class Origin
        {
            External,
            Internal
        };

        Inventory* inventory;
        InventoryView inventoryView;
        bool isOpened;

        //if it is on the ground it will have x,y coordinates specified
        //else it will have specified a parent which eventually recursively will have
        //a parent that lays on the ground or is in the player's container slot
        Origin origin;

        TrackedInventory* parentInventory;

        int x;
        int y;

        TrackedInventory(Inventory* _inventory, Origin _origin, TrackedInventory* _parent, int _x, int _y) :
            inventory(_inventory),
            inventoryView(inventory->createInventoryView()),
            origin(_origin),
            parentInventory(_parent),
            x(_x),
            y(_y)
        {

        }

        bool isExternal() const {return origin == Origin::External;}
        bool isInternal() const {return origin == Origin::Internal;}

        bool operator==(const Inventory* inv) {return inv == inventory;}
        bool operator==(const TrackedInventory& inv) {return inv.inventory == inventory;}

        static TrackedInventory createExternal(Inventory* inventory, int x, int y)
        {
            return TrackedInventory
            (
                inventory,
                Origin::External,
                nullptr,
                x,
                y
            );
        }
        static TrackedInventory createInternal(Inventory* inventory, TrackedInventory* parent)
        {
            return TrackedInventory
            (
                inventory,
                Origin::Internal,
                parent,
                -1,
                -1
            );
        }
    };

    InventorySystem();

    bool tryOpenExternalInventory(Inventory* inventory, int x, int y);
    bool tryOpenInternalInventory(Inventory* inventory, TrackedInventory* parentInventory);
    void closeInventory(Inventory* inventory);
    bool isInventoryOpened(Inventory* inventory);
    bool isInventoryTracked(Inventory* inventory);
    TrackedInventory* findTrackedInventory(Inventory* inventory);

    void abandonInventory(TrackedInventory& inventory);
    bool isParentOfAnyInventory(TrackedInventory& inventory);

    void updatePositionsOfOpenedInventories();

    void onAttemptToInteractWithExternalInventory(const AttemptToInteractWithExternalInventory& event);

    PlayerEquipmentInventory& equipmentInventory();
    const std::vector<TrackedInventory*>& openedInventories() const;

protected:
    PlayerEquipmentInventory m_equipmentInventory;

    std::vector<TrackedInventory> m_trackedInventories;
    std::vector<TrackedInventory*> m_openedInventories;
private:
};

#endif // INVENTORYSYSTEM_H
