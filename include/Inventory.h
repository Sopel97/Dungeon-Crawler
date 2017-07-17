#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>
#include <memory>

#include "SlotContentRequirement.h"

class TileStack;
class WindowSpaceManager;
class InternalWindow;
class InventorySystem;
class InventoryView;
class InventoryWindow;
class Tile;

class Inventory
{
public:

    Inventory();
    virtual ~Inventory();

    virtual std::vector<TileStack>& contents() = 0;
    virtual const std::vector<TileStack>& contents() const = 0;
    virtual const SlotContentRequirement slotContentRequirement(size_t slotId) const = 0;
    virtual std::unique_ptr<InventoryView> createInventoryView(InventorySystem& invSys) = 0;
    virtual std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm, const std::string& name) const = 0;

    virtual TileStack& at(size_t slotId);
    virtual const TileStack& at(size_t slotId) const;
    virtual bool meetsRequirements(const Tile& tile, int slot) const;

    void placeTile(TileStack&& tile, int slot);
    TileStack takeTile(int slot);

    void addTiles(int slot, int count);
    void removeTiles(int slot, int count);
    TileStack splitTiles(int slot, int count);

    virtual int size() const = 0;

protected:
    void onTilePlaced(TileStack& stack, int slot);
    void onTileRemoved(TileStack& stack, int slot);
};

#endif // INVENTORY_H
