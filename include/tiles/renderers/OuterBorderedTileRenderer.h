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
class Spritesheet;

class OuterBorderedTileRenderer : public TileRenderer
{
public:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<Spritesheet> spritesheet;
        TileSpriteSelector spriteSelector;
        ls::Vec2I borderSprites;

        int outerBorderPriority;
    };

    OuterBorderedTileRenderer(Tile& owner, CommonData& commonData);
    OuterBorderedTileRenderer(const OuterBorderedTileRenderer& other, Tile& owner);
    ~OuterBorderedTileRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(SpriteBatch& spriteBatch, const TileLocation& location) const override;
    void drawOutside(SpriteBatch& spriteBatch, const TileLocation& location) const override;

    const sf::Texture& texture() const;
    const Spritesheet& spritesheet() const;

    int outerBorderPriority() const override;
    bool hasOuterBorder() const override;
    bool coversOuterBorders() const override;

    void onTileInstantiated() override;
    void onTileQuantityChanged(int oldQuantity, int newQuantity) override;

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;

protected:
    CommonData* const m_commonData;
    const TimeAnimatedSprite* m_currentAnimatedSprite;
};
