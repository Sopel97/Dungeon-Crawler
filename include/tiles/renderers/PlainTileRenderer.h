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
    PlainTileRenderer(Tile& owner, ComponentCommonData* commonData);
    PlainTileRenderer(const PlainTileRenderer& other, Tile& owner);
    ~PlainTileRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const override;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const InventorySlotView& slot) const override;

    const sf::Texture& texture() const;

    bool coversOuterBorders() const override;
    int outerBorderPriority() const override;

    void onTileInstantiated() override;
    void onTileQuantityChanged(int oldQuantity, int newQuantity) override;

    static std::unique_ptr<ComponentCommonData> createCommonDataStorage();

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;
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
