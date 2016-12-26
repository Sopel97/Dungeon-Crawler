#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include "InventoryView.h"
#include "PlayerEquipmentInventory.h"

#include <list>

class Inventory;
class AttemptToInteractWithExternalInventory;
class TileLocation;

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
            inventoryView(_inventory->createInventoryView()),
            isOpened(true),
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

    InventorySystem(Player& player);

    bool tryOpenExternalInventory(Inventory* inventory, int x, int y);
    bool tryOpenInternalInventory(Inventory* inventory, TrackedInventory* parentInventory);
    void closeInventory(Inventory* inventory);
    bool isInventoryOpened(Inventory* inventory);
    bool isInventoryTracked(Inventory* inventory);
    TrackedInventory* findTrackedInventory(Inventory* inventory);

    void abandonInventory(TrackedInventory& inventory);
    bool isParentOfAnyInventory(TrackedInventory& inventory);

    void updatePositionsOfOpenedInventories();

    bool tryInteractWithExternalInventory(Inventory* inventory, const TileLocation& location);

    PlayerEquipmentInventory& equipmentInventory();
    const std::list<TrackedInventory*>& openedInventories() const;

protected:
    Player& m_player;
    PlayerUi& m_playerUi;

    PlayerEquipmentInventory m_equipmentInventory;

    std::list<TrackedInventory> m_trackedInventories; //not a vector because we have to avoid realocation of inventories because we store pointers to them
    std::list<TrackedInventory*> m_openedInventories;

    void openInventory(TrackedInventory* inventory);
private:
};

#endif // INVENTORYSYSTEM_H
