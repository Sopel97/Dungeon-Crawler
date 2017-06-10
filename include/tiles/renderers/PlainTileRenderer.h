#pragma once

#include "TileRenderer.h"

#include "sprite/TileSpriteSelector.h"
#include "sprite/TimeAnimatedSprite.h"

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
class InventorySlotView;

class PlainTileRenderer : public TileRenderer
{
public:
    PlainTileRenderer();
    PlainTileRenderer(const PlainTileRenderer& other);
    virtual ~PlainTileRenderer();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const;

    const sf::Texture& texture() const;

    virtual bool coversOuterBorders() const;
    virtual int outerBorderPriority() const;

    virtual void onTileInstantiated();
    virtual void onTileQuantityChanged(int oldQuantity, int newQuantity);

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileRenderer> clone() const;
protected:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<sf::Texture> texture;
        ResourceHandle<sf::Texture> metaTexture;
        TileSpriteSelector spriteSelector;

        int outerBorderPriority;
        bool coversOuterBorders;
    };
    CommonData* m_commonData;
    const TimeAnimatedSprite* m_currentAnimatedSprite;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const sf::Texture& texture) const;
};

REGISTER_TILE_RENDERER_TYPE(PlainTileRenderer)
