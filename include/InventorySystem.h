#ifndef INVENTORYSYSTEM_H
#define INVENTORYSYSTEM_H

#include "../LibS/include/Tree.h"
#include "../LibS/include/TreeForest.h"
#include "../LibS/include/Vec2.h"

#include "EventDispatcher.h"
#include "InventoryView.h"
#include "window/InventoryWindow.h"
#include "PlayerEquipmentInventory.h"

#include <list>
#include <optional>

//TODO: rewrite as specified in todo

class Inventory;
class AttemptToInteractWithExternalInventory;
class TileLocation;
class WindowSpaceManager;
class TileMovedFromWorldToWorld;
class TileMovedFromWorldToInventory;
class TileMovedFromInventoryToInventory;
class TileMovedFromInventoryToWorld;
class TileTransferMediator;

class InventorySystem
{
public:
    struct TrackedInventory
    {
        std::string name;
        Inventory* inventory;
        std::unique_ptr<InventoryView> inventoryView;
        InventoryWindow* inventoryWindow;
        std::optional<ls::Vec2I> worldPosition;
        bool isOpened;
        bool isPermanent;

        TrackedInventory(const TrackedInventory&) = delete;
        TrackedInventory(TrackedInventory&&) = default;
        TrackedInventory& operator=(const TrackedInventory&) = delete;
        TrackedInventory& operator=(TrackedInventory&&) = default;

        bool operator==(const Inventory* inv) {return inv == inventory;}
        bool operator==(const TrackedInventory& inv) {return inv.inventory == inventory;}

        static TrackedInventory makeExternal(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inventory, const std::string& name, const ls::Vec2I& pos)
        {
            return TrackedInventory(wsm, invSys, inventory, name, pos);
        }
        static TrackedInventory makeInternal(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inventory, const std::string& name)
        {
            return TrackedInventory(wsm, invSys, inventory, name);
        }
        static TrackedInventory makePermanent(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inventory, const std::string& name)
        {
            return TrackedInventory(wsm, invSys, inventory, name, true);
        }
        void setWindow(InventoryWindow* wnd)
        {
            inventoryWindow = wnd;
            isOpened = true;
        }
        void resetWindow()
        {
            inventoryWindow = nullptr;
            isOpened = false;
        }
        std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm)
        {
            return inventory->createInventoryWindow(wsm, name);
        }
    private:
        TrackedInventory(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inv, const std::string& name, bool perm = false) :
            name(name),
            inventory(&inv),
            inventoryView(inv.createInventoryView(invSys)),
            inventoryWindow(nullptr),
            worldPosition(std::nullopt),
            isOpened(true),
            isPermanent(perm)
        {
        }
        TrackedInventory(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inv, const std::string& name, const ls::Vec2I& pos) :
            name(name),
            inventory(&inv),
            inventoryView(inv.createInventoryView(invSys)),
            inventoryWindow(nullptr),
            worldPosition(pos),
            isOpened(true),
            isPermanent(false)
        {
        }
    };

    using TrackedInventoryTree = ls::Tree<TrackedInventory>;
    using TrackedInventoryForest = ls::TreeForest<TrackedInventoryTree>;
    using TrackedInventoryTreeHandle = ls::TreeForest<TrackedInventoryTree>::TreeHandle;
    using ConstTrackedInventoryTreeHandle = ls::TreeForest<TrackedInventoryTree>::ConstTreeHandle;
    using TrackedInventoryHandle = typename TrackedInventoryTree::Iterator;
    using ConstTrackedInventoryHandle = typename TrackedInventoryTree::ConstIterator;

    InventorySystem(WindowSpaceManager& wsm, Player& player, TileTransferMediator& tileTransferMediator);

    bool tryOpenExternalInventory(Tile& tile, Inventory& inventory, const ls::Vec2I& pos);
    bool tryOpenInternalInventory(Tile& tile, Inventory& inventory, const Inventory& parentInventory);
    void openPermanentInventory(Inventory& inventory, const std::string& name);
    void closeInventory(Inventory& inventory);
    bool isInventoryOpened(const Inventory& inventory);
    bool isInventoryTracked(const Inventory& inventory);
    std::pair<TrackedInventoryTreeHandle, TrackedInventoryHandle> find(const Inventory& inventory);
    std::pair<ConstTrackedInventoryTreeHandle, ConstTrackedInventoryHandle> find(const Inventory& inventory) const;

    bool tryInteractWithExternalInventory(Tile& tile, Inventory& inventory, const TileLocation& location);
    bool tryInteractWithInternalInventory(Tile& tile, Inventory& inventory, const InventorySlotView& slot);

    bool canStore(const Inventory& inventory, const Tile& tile) const;

    PlayerEquipmentInventory& equipmentInventory();

    TileTransferMediator& tileTransferMediator();

    Player& player();

    void onTileMovedFromWorldToWorld(const TileMovedFromWorldToWorld& event);
    void onTileMovedFromWorldToInventory(const TileMovedFromWorldToInventory& event);
    void onTileMovedFromInventoryToInventory(const TileMovedFromInventoryToInventory& event);
    void onTileMovedFromInventoryToWorld(const TileMovedFromInventoryToWorld& event);

    void onInventoryWindowClosed(Inventory& inventory);

protected:
    WindowSpaceManager& m_wsm;
    Player& m_player;
    PlayerUi& m_playerUi;
    TileTransferMediator& m_tileTransferMediator;

    TrackedInventoryForest m_trackedInventories;

    PlayerEquipmentInventory m_equipmentInventory;

    EventDispatcher::EventCallbackHandle<TileMovedFromWorldToWorld> m_tileMovedFromWorldToWorldEventSubscription;
    EventDispatcher::EventCallbackHandle<TileMovedFromWorldToInventory> m_tileMovedFromWorldToInventoryEventSubscription;
    EventDispatcher::EventCallbackHandle<TileMovedFromInventoryToInventory> m_tileMovedFromInventoryToInventoryEventSubscription;
    EventDispatcher::EventCallbackHandle<TileMovedFromInventoryToWorld> m_tileMovedFromInventoryToWorldEventSubscription;

    void openTrackedInventory(TrackedInventoryHandle inventory);
    void abandonInventory(TrackedInventoryTreeHandle tree, TrackedInventoryHandle inventory);

    void closeInventory(TrackedInventoryHandle& inventoryHandle);
private:
};

#endif // INVENTORYSYSTEM_H
