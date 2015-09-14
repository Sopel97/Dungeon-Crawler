#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>

class Tile;
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

    virtual const std::vector<Tile*>& contents() const = 0;
    virtual const ContentRequirement slotContentRequirement(int slotId) const = 0;
    virtual InventoryView createInventoryView() = 0;

    virtual int insert(Tile* tile, int count = -1); //returns number of inserted elements. -1 count means that it will insert all
    virtual int insert(Tile* tile, int slotId, int count = -1); //same as above

    virtual int erase(Tile* tile, int count = -1); //same as above
    virtual int erase(Tile* tile, int slotId, int count = -1); //same as above

    virtual int numberOfSlots() const = 0;

protected:

};

#endif // INVENTORY_H
