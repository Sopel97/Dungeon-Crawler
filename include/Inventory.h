#ifndef INVENTORY_H
#define INVENTORY_H

#include <vector>

class Tile;
class InventoryView;

class Inventory
{
public:
    Inventory();
    virtual ~Inventory();

    virtual const std::vector<Tile*>& contents() const = 0;
    virtual InventoryView createInventoryView() const = 0;
protected:

};

#endif // INVENTORY_H
