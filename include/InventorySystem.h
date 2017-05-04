#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include "../LibS/include/Tree.h"
#include "../LibS/include/TreeForest.h"
#include "../LibS/include/Vec2.h"

#include "InventoryView.h"
#include "PlayerEquipmentInventory.h"

#include <list>
#include <optional>

//TODO: rewrite as specified in todo

class Inventory;
class AttemptToInteractWithExternalInventory;
class TileLocation;

class InventorySystem
{
public:
    struct TrackedInventory
    {
        Inventory* inventory;
        InventoryView inventoryView;
        std::optional<ls::Vec2I> worldPosition;
        bool isOpened;
        bool isPermanent;

        bool operator==(const Inventory* inv) {return inv == inventory;}
        bool operator==(const TrackedInventory& inv) {return inv.inventory == inventory;}

        static TrackedInventory makeExternal(Inventory& inventory, const ls::Vec2I& pos)
        {
            return TrackedInventory(inventory, pos);
        }
        static TrackedInventory makeInternal(Inventory& inventory)
        {
            return TrackedInventory(inventory);
        }
        static TrackedInventory makePermanent(Inventory& inventory)
        {
            return TrackedInventory(inventory, true);
        }
    private:
        TrackedInventory(Inventory& inv, bool perm = false) :
            inventory(&inv),
            inventoryView(inv.createInventoryView()),
            worldPosition(std::nullopt),
            isOpened(true),
            isPermanent(perm)
        {

        }
        TrackedInventory(Inventory& inv, const ls::Vec2I& pos) :
            inventory(&inv),
            inventoryView(inv.createInventoryView()),
            worldPosition(pos),
            isOpened(true),
            isPermanent(false)
        {

        }
    };

    using TrackedInventoryTree = ls::Tree<TrackedInventory>;
    using TrackedInventoryForest = ls::TreeForest<TrackedInventoryTree>;
    using TrackedInventoryHandle = typename TrackedInventoryTree::Iterator;
    using ConstTrackedInventoryHandle = typename TrackedInventoryTree::ConstIterator;

    InventorySystem(Player& player);

    bool tryOpenExternalInventory(Inventory& inventory, const ls::Vec2I& pos);
    bool tryOpenInternalInventory(Inventory& inventory, Inventory& parentInventory);
    void openPermanentInventory(Inventory& inventory);
    void closeInventory(Inventory& inventory);
    bool isInventoryOpened(Inventory& inventory);
    bool isInventoryTracked(Inventory& inventory);
    TrackedInventoryHandle find(Inventory& inventory);

    bool tryInteractWithExternalInventory(Inventory& inventory, const TileLocation& location);

    PlayerEquipmentInventory& equipmentInventory();

protected:
    Player& m_player;
    PlayerUi& m_playerUi;

    PlayerEquipmentInventory m_equipmentInventory;

    TrackedInventoryForest m_trackedInventories;

    void openTrackedInventory(TrackedInventoryHandle inventory);
    void abandonInventory(TrackedInventoryHandle inventory);
private:
};

#endif // INVENTORYSYSTEM_H
