#pragma once

#include "ResourceManager.h"

#include "Inventory.h"

#include "../LibS/Geometry.h"

#include <map>

class TileStack;

namespace sf
{
    class RenderStates;
    class RenderTarget;
    class Texture;
}

class InventorySlotView
{
public:

    InventorySlotView(Inventory* inventory, size_t slotId, const ls::Vec2I& position);

    InventorySlotView(const InventorySlotView&) = default;
    InventorySlotView(InventorySlotView&&) = default;

    InventorySlotView& operator=(const InventorySlotView&) = default;
    InventorySlotView& operator=(InventorySlotView&&) = default;

    TileStack* content() const;
    const ls::Vec2I& position() const;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, Inventory::ContentRequirement contentRequirement);

    static const ls::Vec2I& slotSize();

protected:
    static ResourceHandle<sf::Texture> m_texture;
    static ls::Vec2I m_slotTexture;
    static ls::Vec2I m_slotTextureSize;
    static ls::Vec2I m_requirementIconSize;
    static std::map<Inventory::ContentRequirement, ls::Vec2I> m_requirementIcons;

    Inventory* m_inventory; //inventory will always outlive the views of its slots
    size_t m_slotId;
    ls::Vec2I m_position; //relative to inventory inner region
};
