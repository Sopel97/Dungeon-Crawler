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
    OuterBorderedTileRenderer(Tile& owner);
    OuterBorderedTileRenderer(const OuterBorderedTileRenderer& other, Tile& owner);
    ~OuterBorderedTileRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const override;
    void drawOutside(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const override;

    const sf::Texture& texture() const;

    int outerBorderPriority() const override;
    bool hasOuterBorder() const override;
    bool coversOuterBorders() const override;

    void onTileInstantiated() override;
    void onTileQuantityChanged(int oldQuantity, int newQuantity) override;

    std::unique_ptr<ComponentCommonData> createCommonDataStorage() const override;
    void setCommonDataStorage(ComponentCommonData& commonData) override;

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;
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
