#ifndef INVENTORYSLOTVIEW_H
#define INVENTORYSLOTVIEW_H

#include "ResourceManager.h"

#include "Inventory.h"

#include "../LibS/Geometry.h"

#include <map>

class Tile;
namespace sf
{
    class RenderStates;
    class RenderTarget;
    class Texture;
}

class InventorySlotView
{
public:

    InventorySlotView(Tile* content, const ls::Vec2I& position);

    InventorySlotView(const InventorySlotView& other);
    InventorySlotView(InventorySlotView&& other);

    InventorySlotView& operator=(const InventorySlotView& other);
    InventorySlotView& operator=(InventorySlotView&& other);

    Tile* content() const;
    const ls::Vec2I& position() const;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Inventory::ContentRequirement contentRequirement);

    static const ls::Vec2I& slotSize();

protected:
    static ResourceHandle<sf::Texture> m_texture;
    static ls::Vec2I m_slotTexture;
    static ls::Vec2I m_slotTextureSize;
    static ls::Vec2I m_requirementIconSize;
    static std::map<Inventory::ContentRequirement, ls::Vec2I> m_requirementIcons;

    Tile* m_content;
    ls::Vec2I m_position; //relative to inventory inner region
};

#endif // INVENTORYSLOTVIEW_H
