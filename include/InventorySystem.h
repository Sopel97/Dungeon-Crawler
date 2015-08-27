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

        TrackedInventory(Inventory* _inventory, Origin _origin, TrackedInventory* _parent, int _x, int _y) :
            inventory(_inventory),
            inventoryView(inventory->createInventoryView()),
            isOpened(true),
            origin(_origin),
            parentInventory(_parent),
            x(_x),
            y(_y)
        {

        }

        Inventory* inventory;
        InventoryView inventoryView; //TODO: set view position when opening inventory
        bool isOpened;

        //if it is on the ground it will have x,y coordinates specified
        //else it will have specified a parent which eventually recursively will have
        //a parent that lays on the ground or is in the player's container slot
        Origin origin;
        TrackedInventory* parentInventory;
        int x;
        int y;

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
    bool tryOpenInternalInventory(Inventory* inventory, TrackedInventory& parentInventory);
    void closeInventory(Inventory* inventory);
    bool isInventoryOpened(Inventory* inventory);
    bool isInventoryTracked(Inventory* inventory);
    TrackedInventory* findInventory(Inventory* inventory);

    bool tryReopenTrackedInventory(TrackedInventory& inventory);
    void moveInventoryToTheEnd(TrackedInventory& inventory);
    void abandonInventory(TrackedInventory& inventory);
    bool isParentOfAnyInventory(TrackedInventory& inventory);

    void onAttemptToInteractWithExternalInventory(const AttemptToInteractWithExternalInventory& event);

    PlayerEquipmentInventory& equipmentInventory();
    std::vector<TrackedInventory*> openedInventories();

protected:
    PlayerEquipmentInventory m_equipmentInventory;

    std::vector<TrackedInventory> m_trackedInventories;
private:
};

#endif // INVENTORYSYSTEM_H
