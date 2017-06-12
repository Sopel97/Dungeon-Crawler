#pragma once

#include "TileRenderer.h"

#include "ResourceManager.h"

#include "ResourceLoaders.h"

#include "../LibS/Geometry.h"

#include "../../ComponentCommonData.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class MapLayer;
class Tile;
class TileLocation;

class InnerBorderedWallTileRenderer : public TileRenderer
{
public:
    InnerBorderedWallTileRenderer(Tile& owner);
    InnerBorderedWallTileRenderer(const InnerBorderedWallTileRenderer& other, Tile& owner);
    ~InnerBorderedWallTileRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const override;
    void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const override;

    const sf::Texture& texture() const;
    int innerBorderGroup() const override;

    bool isTall() const override;

    std::unique_ptr<ComponentCommonData> createCommonDataStorage() const override;
    void setCommonDataStorage(ComponentCommonData& commonData) override;

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;
protected:
    struct CommonData : public ComponentCommonData
    {
        struct SpriteSet
        {
            //specific names are based on appearance in file "inner_bordered_wall_sprites_preview.png"
            ls::Vec2I full;
            ls::Vec2I top;
            ls::Vec2I left;
            ls::Vec2I topLeft;
            ls::Vec2I bottomRight;

            ls::Vec2I outerLeft;
            ls::Vec2I innerTopRight;
            ls::Vec2I innerBottomLeft;
            ls::Vec2I outerTopRight;
            ls::Vec2I outerBottomLeft;
            ls::Vec2I outerTop;
            ls::Vec2I outerTopLeft;
            ls::Vec2I innerBottomRight;

        }spriteSet;

        ResourceHandle<sf::Texture> texture;
        ResourceHandle<sf::Texture> metaTexture;

        int innerBorderGroup;
    };
    CommonData* m_commonData;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const sf::Texture& texture) const;
};
