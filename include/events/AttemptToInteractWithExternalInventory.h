#ifndef ATTEMPTTOINTERACTWITHEXTERNALINVENTORY_H
#define ATTEMPTTOINTERACTWITHEXTERNALINVENTORY_H

#include "EventDispatcher.h"

class Inventory;

class AttemptToInteractWithExternalInventory : public Event
{
public:
    AttemptToInteractWithExternalInventory(Inventory* inventory, int x, int y);

    Inventory* inventory() const;
    int x() const;
    int y() const;
protected:
    Inventory* m_inventory;
    int m_x;
    int m_y;
};

#endif // ATTEMPTTOINTERACTWITHEXTERNALINVENTORY_H
