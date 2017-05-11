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
class WindowSpaceManager;

class InventorySystem
{
public:
    struct TrackedInventory
    {
        Inventory* inventory;
        std::unique_ptr<InventoryWindow> inventoryWindow;
        std::unique_ptr<InventoryView> inventoryView;
        std::optional<ls::Vec2I> worldPosition;
        bool isOpened;
        bool isPermanent;

        TrackedInventory(const TrackedInventory&) = delete;
        TrackedInventory(TrackedInventory&&) = default;
        TrackedInventory& operator=(const TrackedInventory&) = delete;
        TrackedInventory& operator=(TrackedInventory&&) = default;

        bool operator==(const Inventory* inv) {return inv == inventory;}
        bool operator==(const TrackedInventory& inv) {return inv.inventory == inventory;}

        static TrackedInventory makeExternal(WindowSpaceManager& wsm, Inventory& inventory, const ls::Vec2I& pos)
        {
            return TrackedInventory(wsm, inventory, pos);
        }
        static TrackedInventory makeInternal(WindowSpaceManager& wsm, Inventory& inventory)
        {
            return TrackedInventory(wsm, inventory);
        }
        static TrackedInventory makePermanent(WindowSpaceManager& wsm, Inventory& inventory)
        {
            return TrackedInventory(wsm, inventory, true);
        }
    private:
        TrackedInventory(WindowSpaceManager& wsm, Inventory& inv, bool perm = false) :
            inventory(&inv),
            inventoryWindow(inv.createInventoryWindow(wsm)),
            inventoryView(inv.createInventoryView(*inventoryWindow)),
            worldPosition(std::nullopt),
            isOpened(true),
            isPermanent(perm)
        {
            inventoryWindow->updateMaxContentHeight(*inventoryView);
        }
        TrackedInventory(WindowSpaceManager& wsm, Inventory& inv, const ls::Vec2I& pos) :
            inventory(&inv),
            inventoryWindow(inv.createInventoryWindow(wsm)),
            inventoryView(inv.createInventoryView(*inventoryWindow)),
            worldPosition(pos),
            isOpened(true),
            isPermanent(false)
        {
            inventoryWindow->updateMaxContentHeight(*inventoryView);
        }
    };

    using TrackedInventoryTree = ls::Tree<TrackedInventory>;
    using TrackedInventoryForest = ls::TreeForest<TrackedInventoryTree>;
    using TrackedInventoryTreeHandle = ls::TreeForest<TrackedInventoryTree>::TreeHandle;
    using TrackedInventoryHandle = typename TrackedInventoryTree::Iterator;
    using ConstTrackedInventoryHandle = typename TrackedInventoryTree::ConstIterator;

    InventorySystem(WindowSpaceManager& wsm, Player& player);

    bool tryOpenExternalInventory(Inventory& inventory, const ls::Vec2I& pos);
    bool tryOpenInternalInventory(Inventory& inventory, Inventory& parentInventory);
    void openPermanentInventory(Inventory& inventory);
    void closeInventory(Inventory& inventory);
    bool isInventoryOpened(Inventory& inventory);
    bool isInventoryTracked(Inventory& inventory);
    std::pair<TrackedInventoryTreeHandle, TrackedInventoryHandle> find(Inventory& inventory);

    bool tryInteractWithExternalInventory(Inventory& inventory, const TileLocation& location);

    PlayerEquipmentInventory& equipmentInventory();

protected:
    WindowSpaceManager& m_wsm;
    Player& m_player;
    PlayerUi& m_playerUi;

    PlayerEquipmentInventory m_equipmentInventory;

    TrackedInventoryForest m_trackedInventories;

    void openTrackedInventory(TrackedInventoryHandle inventory);
    void abandonInventory(TrackedInventoryTreeHandle tree, TrackedInventoryHandle inventory);
private:
};

#endif // INVENTORYSYSTEM_H
