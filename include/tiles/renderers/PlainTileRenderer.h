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
class Spritesheet;

class PlainTileRenderer : public TileRenderer
{
public:
    struct CommonData : public ComponentCommonData
    {
        ResourceHandle<Spritesheet> spritesheet;
        TileSpriteSelector spriteSelector;
        int spriteSize;
        bool hasMetaTexture;

        int outerBorderPriority;
        bool coversOuterBorders;
    };

    PlainTileRenderer(Tile& owner, CommonData& commonData);
    PlainTileRenderer(const PlainTileRenderer& other, Tile& owner);
    ~PlainTileRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(SpriteBatch& mainSriteBatch, SpriteBatch& metaSpriteBatch, const TileLocation& location) const override;

    void draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates, const InventorySlotView& slot) const override;

    const sf::Texture& texture() const;
    const Spritesheet& spritesheet() const;

    bool coversOuterBorders() const override;
    int outerBorderPriority() const override;

    void onTileInstantiated() override;
    void onTileQuantityChanged(int oldQuantity, int newQuantity) override;

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;
    const TimeAnimatedSprite* m_currentAnimatedSprite;
};
