#pragma once

#include "TileRenderer.h"

#include "QuantityBasedSpriteSet.h"

#include "ResourceManager.h"

#include "ResourceLoaders.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

#include <vector>

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class MapLayer;
class Tile;
class TileLocation;

class PlainQuantityBasedTileRenderer : public TileRenderer
{
public:
	PlainQuantityBasedTileRenderer();
	PlainQuantityBasedTileRenderer(const PlainQuantityBasedTileRenderer& other);
    virtual ~PlainQuantityBasedTileRenderer();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const;

    const sf::Texture& texture() const;

    virtual bool coversOuterBorders() const;
    virtual int outerBorderPriority() const;

    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);
    virtual void onTileInstantiated();

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileRenderer> clone() const;
protected:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<sf::Texture> texture;
        QuantityBasedSpriteSet spriteSet;
        std::vector<ls::Vec2I> respectiveMetaSprites;

        int outerBorderPriority;
        bool coversOuterBorders;
    };
    CommonData* m_commonData;
    int m_spriteId;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const ls::Vec2I& sprite) const;
};

REGISTER_TILE_RENDERER_TYPE(PlainQuantityBasedTileRenderer)
