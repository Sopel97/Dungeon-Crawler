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

        TileInnerBorderGroupType innerBorderGroup;

        bool hasMetaTexture;
    };

    InnerBorderedWallTileRenderer(Tile& owner, CommonData& commonData);
    InnerBorderedWallTileRenderer(const InnerBorderedWallTileRenderer& other, Tile& owner);
    ~InnerBorderedWallTileRenderer() override;

    void loadFromConfiguration(ConfigurationNode& config) override;

    void draw(SpriteBatch& spriteBatch, const TileLocation& location) const override;
    void drawMeta(SpriteBatch& spriteBatch, const TileLocation& location) const override;

    const sf::Texture& texture() const;
    TileInnerBorderGroupType innerBorderGroup() const override;

    std::unique_ptr<TileRenderer> clone(Tile& owner) const override;
protected:
    CommonData* const m_commonData;

    virtual void draw(SpriteBatch& spriteBatch, const TileLocation& location, const ls::Vec2I& textureOffset) const;
};
