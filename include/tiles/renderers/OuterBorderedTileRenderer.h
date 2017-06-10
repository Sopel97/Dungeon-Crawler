#pragma once

#include "TileRenderer.h"

#include "ResourceManager.h"

#include "ResourceLoaders.h"

#include "sprite/TileSpriteSelector.h"
#include "sprite/TimeAnimatedSprite.h"

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

class OuterBorderedTileRenderer : public TileRenderer
{
public:
    OuterBorderedTileRenderer();
    OuterBorderedTileRenderer(const OuterBorderedTileRenderer& other);
    virtual ~OuterBorderedTileRenderer();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const sf::Texture& texture() const;

    virtual int outerBorderPriority() const;
    virtual bool hasOuterBorder() const;
    virtual bool coversOuterBorders() const;

    virtual void onTileInstantiated();
    void onTileQuantityChanged(int oldQuantity, int newQuantity) override;

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileRenderer> clone() const;
protected:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<sf::Texture> texture;
        TileSpriteSelector spriteSelector;
        ls::Vec2I borderSprites;

        int outerBorderPriority;
    };
    CommonData* m_commonData;
    const TimeAnimatedSprite* m_currentAnimatedSprite;
};

REGISTER_TILE_RENDERER_TYPE(OuterBorderedTileRenderer)
