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
    InnerBorderedWallTileRenderer();
    InnerBorderedWallTileRenderer(const InnerBorderedWallTileRenderer& other);
    virtual ~InnerBorderedWallTileRenderer();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;
    virtual void drawMeta(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location) const;

    const sf::Texture& texture() const;
    virtual int innerBorderGroup() const;

    virtual bool isTall() const;

    virtual std::unique_ptr<ComponentCommonData> createCommonDataStorage() const;
    virtual void setCommonDataStorage(ComponentCommonData& commonData);

    virtual std::unique_ptr<TileRenderer> clone() const;
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

        }mainSprites, metaSprites;
        ResourceHandle<sf::Texture> texture;

        int innerBorderGroup;
    };
    CommonData* m_commonData;

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const TileLocation& location, const CommonData::SpriteSet& spriteSet) const;
};

REGISTER_TILE_RENDERER_TYPE(InnerBorderedWallTileRenderer)
