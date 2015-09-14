#include "events/AttemptToInteractWithExternalInventory.h"

AttemptToInteractWithExternalInventory::AttemptToInteractWithExternalInventory(Inventory* inventory, int x, int y) :
    m_inventory(inventory),
    m_x(x),
    m_y(y)
{

}

Inventory* AttemptToInteractWithExternalInventory::inventory() const
{
    return m_inventory;
}
int AttemptToInteractWithExternalInventory::x() const
{
    return m_x;
}
int AttemptToInteractWithExternalInventory::y() const
{
    return m_y;
}
