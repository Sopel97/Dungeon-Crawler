#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>

class TileStack;
class InventoryView;

class Inventory
{
public:
    enum class ContentRequirement
    {
        None,
        Helmet,
        Chestplate,
        Pants,
        Boots,
        Sword,
        Shield,
        Necklace,
        Ring,
        Ammo,
        Container
    };

    Inventory();
    virtual ~Inventory();

    virtual std::vector<TileStack*>& contents() = 0;
    virtual const ContentRequirement slotContentRequirement(size_t slotId) const = 0;
    virtual InventoryView createInventoryView() = 0;

    virtual TileStack* at(size_t slotId);

    virtual void deleteTileAt(size_t slotId);

    virtual int insert(TileStack* tile, int count = -1); //returns number of inserted elements. -1 count means that it will insert all
    virtual int insert(TileStack* tile, size_t slotId, int count = -1); //same as above

    virtual int erase(TileStack* tile, int count = -1); //same as above
    virtual int erase(TileStack* tile, size_t slotId, int count = -1); //same as above

    virtual int numberOfSlots() const = 0;

protected:

};

#endif // INVENTORY_H
