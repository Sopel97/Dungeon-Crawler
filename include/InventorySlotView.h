#ifndef INVENTORYSLOTVIEW_H
#define INVENTORYSLOTVIEW_H

#include "ResourceManager.h"

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

    InventorySlotView(Tile* content, const ls::Vec2I& position, ContentRequirement requirement = ContentRequirement::None);

    InventorySlotView(const InventorySlotView& other);
    InventorySlotView(InventorySlotView&& other);

    InventorySlotView& operator=(const InventorySlotView& other);
    InventorySlotView& operator=(InventorySlotView&& other);

    bool setContent(Tile* newContent);
    bool isValidContent(Tile* tile) const;
    bool isEmpty() const;
    Tile*& content() const;
    Tile* releaseContent();
    const ls::Vec2I& position() const;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

    static const ls::Vec2I& slotSize();

protected:
    static ResourceHandle<sf::Texture> m_texture;
    static ls::Vec2I m_slotTexture;
    static ls::Vec2I m_slotTextureSize;
    static ls::Vec2I m_requirementIconSize;
    static std::map<ContentRequirement, ls::Vec2I> m_requirementIcons;

    Tile** m_content; //pointer to pointer in inventory that owns the tile. It is to allow moving tiles through view.
    ls::Vec2I m_position; //relative to inventory inner region
    ContentRequirement m_contentRequirement;
};

#endif // INVENTORYSLOTVIEW_H
