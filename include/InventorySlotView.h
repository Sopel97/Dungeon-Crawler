#ifndef INVENTORYSLOTVIEW_H
#define INVENTORYSLOTVIEW_H

#include "ResourceManager.h"

#include "../LibS/GeometryLight.h"

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

    InventorySlotView(Tile*& content, const Geo::Vec2I& position, ContentRequirement requirement = ContentRequirement::None);

    bool setContent(Tile* newContent);
    bool isValidContent(Tile* tile) const;
    bool isEmpty() const;
    Tile const* content() const;
    Tile* releaseContent();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);

protected:
    static ResourceHandle<sf::Texture> m_texture;
    static Geo::Vec2I m_slotTexture;
    static Geo::Vec2I m_slotTextureSize;
    static Geo::Vec2I m_requirementIconSize;
    static std::map<ContentRequirement, Geo::Vec2I> m_requirementIcons;

    Tile*& m_content; //reference to pointer in inventory that owns the tile. It is to allow moving tiles through view.
    Geo::Vec2I m_position; //relative to inventory inner region
    ContentRequirement m_contentRequirement;
};

#endif // INVENTORYSLOTVIEW_H
