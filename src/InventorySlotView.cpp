#include "InventorySlotView.h"

using namespace Geo;

InventorySlotView::InventorySlotView(Tile*& content, const Vec2I& position, ContentRequirement requirement) :
    m_content(content),
    m_position(position),
    m_contentRequirement(requirement)
{
    //ctor
}

bool InventorySlotView::setContent(Tile* newContent)
{
    if(isValidContent(newContent))
    {
        if(m_content != nullptr) delete m_content;
        m_content = newContent;
        return true;
    }
    else return false;
}
bool InventorySlotView::isValidContent(Tile* tile) const
{
    return true; //later compare with rquirements
}
bool InventorySlotView::isEmpty() const
{
    return m_content == nullptr;
}
Tile const* InventorySlotView::content() const
{
    return m_content;
}
Tile* InventorySlotView::releaseContent()
{
    Tile* content = m_content;
    m_content = nullptr;
    return content;
}
