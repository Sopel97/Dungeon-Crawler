#pragma once

#include "ResourceManager.h"

#include "../LibS/Geometry.h"

#include "InventorySlotLocation.h"

#include <map>

class TileStack;
class Inventory;
enum class SlotContentRequirement;

namespace sf
{
    class RenderStates;
    class RenderTarget;
    class Texture;
    class Font;
}

class Spritesheet;

class InventorySlotView
{
public:
    static constexpr int tileSize = 32;

    InventorySlotView(Inventory* inventory, size_t slotId, const ls::Vec2I& position);

    InventorySlotView(const InventorySlotView&) = default;
    InventorySlotView(InventorySlotView&&) = default;

    InventorySlotView& operator=(const InventorySlotView&) = default;
    InventorySlotView& operator=(InventorySlotView&&) = default;

    TileStack& content();
    const TileStack& content() const;
    const ls::Vec2I& position() const;
    ls::Vec2I center() const;
    size_t slotId() const;
    Inventory& inventory();
    const Inventory& inventory() const;

    void draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates);

    static const ls::Vec2I& slotSize();

    InventorySlotLocation location() const;

protected:
    static constexpr int m_fontSize = 16;

    static ResourceHandle<Spritesheet> m_spritesheet;
    static ResourceHandle<sf::Font> m_font;
    static ls::Vec2I m_slotTexture;
    static ls::Vec2I m_slotTextureSize;
    static ls::Vec2I m_requirementIconSize;
    static std::map<SlotContentRequirement, ls::Vec2I> m_requirementIcons;

    Inventory* m_inventory; //inventory will always outlive the views of its slots
    size_t m_slotId;
    ls::Vec2I m_position; //relative to inventory inner region

    void drawQuantity(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, int quantity);
};
