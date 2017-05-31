#ifndef CONTAINERINVENTORY_H
#define CONTAINERINVENTORY_H

#include <vector>
#include <memory>

#include "Inventory.h"
#include "tiles/TileStack.h"

class Tile;
class InventoryView;
class WindowSpaceManager;
class InternalWindow;
class InventorySystem;
class InventoryWindow;

class ContainerInventory : public Inventory
{
public:
    ContainerInventory(int size);
    ContainerInventory(const ContainerInventory& other);

    std::vector<TileStack>& contents() override;
    const std::vector<TileStack>& contents() const override;
    const SlotContentRequirement slotContentRequirement(size_t slotId) const override;
    std::unique_ptr<InventoryView> createInventoryView(InventorySystem& invSys) override;
    std::unique_ptr<InventoryWindow> createInventoryWindow(WindowSpaceManager& wsm, const std::string& name) const override;

    void setSize(int newSize);

    int size() const override;

protected:
private:
    std::vector<TileStack> m_contents;
};

#endif // CONTAINERINVENTORY_H
