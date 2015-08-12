#ifndef INVENTORYSLOTVIEW_H
#define INVENTORYSLOTVIEW_H

#include "../LibS/GeometryLight.h"

class Tile;

class InventorySlotView
{
public:
    enum class TileRequirement
    {
        None
    };

    InventorySlotView(Tile*& content, const Geo::Vec2I& position, TileRequirement requirement = TileRequirement::None);

    bool setContent(Tile* newContent);
    bool isValidContent(Tile* tile) const;
    bool isEmpty() const;
    Tile const* content() const;
    Tile* releaseContent();

protected:
    Tile*& m_content; //reference to pointer in inventory that owns the tile. It is to allow moving tiles through view.
    Geo::Vec2I m_position; //relative to inventory inner region
    TileRequirement m_contentRequirement;
};

#endif // INVENTORYSLOTVIEW_H
