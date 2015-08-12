#ifndef INVENTORYSLOTVIEW_H
#define INVENTORYSLOTVIEW_H

#include "../LibS/GeometryLight.h"

class Tile;

class InventorySlotView
{
public:
    enum class ContentRequirement
    {
        None,
        Head,
        Chest,
        Legs,
        Feet,
        Hand,
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

protected:
    Tile*& m_content; //reference to pointer in inventory that owns the tile. It is to allow moving tiles through view.
    Geo::Vec2I m_position; //relative to inventory inner region
    ContentRequirement m_contentRequirement;
};

#endif // INVENTORYSLOTVIEW_H
