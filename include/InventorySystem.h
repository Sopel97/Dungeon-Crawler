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
class TileTransferMediator;

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

        static TrackedInventory makeExternal(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inventory, const ls::Vec2I& pos)
        {
            return TrackedInventory(wsm, invSys, inventory, pos);
        }
        static TrackedInventory makeInternal(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inventory)
        {
            return TrackedInventory(wsm, invSys, inventory);
        }
        static TrackedInventory makePermanent(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inventory)
        {
            return TrackedInventory(wsm, invSys, inventory, true);
        }
    private:
        TrackedInventory(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inv, bool perm = false) :
            inventory(&inv),
            inventoryWindow(inv.createInventoryWindow(wsm)),
            inventoryView(inv.createInventoryView(invSys, *inventoryWindow)),
            worldPosition(std::nullopt),
            isOpened(true),
            isPermanent(perm)
        {
            inventoryWindow->updateMaxContentHeight(*inventoryView);
            inventoryWindow->headerButton(InternalWindow::closeButtonId()).setCallback([&invSys](InternalWindow& wnd) { invSys.closeInventory(wnd); });
        }
        TrackedInventory(WindowSpaceManager& wsm, InventorySystem& invSys, Inventory& inv, const ls::Vec2I& pos) :
            inventory(&inv),
            inventoryWindow(inv.createInventoryWindow(wsm)),
            inventoryView(inv.createInventoryView(invSys, *inventoryWindow)),
            worldPosition(pos),
            isOpened(true),
            isPermanent(false)
        {
            inventoryWindow->updateMaxContentHeight(*inventoryView);
            inventoryWindow->headerButton(InternalWindow::closeButtonId()).setCallback([&invSys](InternalWindow& wnd) { invSys.closeInventory(wnd); });
        }
    };

    using TrackedInventoryTree = ls::Tree<TrackedInventory>;
    using TrackedInventoryForest = ls::TreeForest<TrackedInventoryTree>;
    using TrackedInventoryTreeHandle = ls::TreeForest<TrackedInventoryTree>::TreeHandle;
    using TrackedInventoryHandle = typename TrackedInventoryTree::Iterator;
    using ConstTrackedInventoryHandle = typename TrackedInventoryTree::ConstIterator;

    InventorySystem(WindowSpaceManager& wsm, Player& player, TileTransferMediator& tileTransferMediator);

    bool tryOpenExternalInventory(Inventory& inventory, const ls::Vec2I& pos);
    bool tryOpenInternalInventory(Inventory& inventory, Inventory& parentInventory);
    void openPermanentInventory(Inventory& inventory);
    void closeInventory(Inventory& inventory);
    bool isInventoryOpened(const Inventory& inventory);
    bool isInventoryTracked(const Inventory& inventory);
    std::pair<TrackedInventoryTreeHandle, TrackedInventoryHandle> find(const Inventory& inventory);

    bool tryInteractWithExternalInventory(Inventory& inventory, const TileLocation& location);

    PlayerEquipmentInventory& equipmentInventory();

    TileTransferMediator& tileTransferMediator();

    void onTileMovedFromWorldToWorld(const TileMovedFromWorldToWorld& event);

protected:
    WindowSpaceManager& m_wsm;
    Player& m_player;
    PlayerUi& m_playerUi;
    TileTransferMediator& m_tileTransferMediator;

    TrackedInventoryForest m_trackedInventories;

    PlayerEquipmentInventory m_equipmentInventory;

    EventDispatcher::EventCallbackHandle<TileMovedFromWorldToWorld> m_tileMovedFromWorldToWorldEventSubscription;

    void openTrackedInventory(TrackedInventoryHandle inventory);
    void abandonInventory(TrackedInventoryTreeHandle tree, TrackedInventoryHandle inventory);

    std::pair<TrackedInventoryTreeHandle, TrackedInventoryHandle> find(const InternalWindow& wnd);
    void closeInventory(const std::pair<TrackedInventoryTreeHandle, TrackedInventoryHandle>& findResult);
    void closeInventory(InternalWindow& wnd);
private:
};

#endif // INVENTORYSYSTEM_H
