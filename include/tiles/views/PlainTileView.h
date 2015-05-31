#ifndef PLAINTILEVIEW_H
#define PLAINTILEVIEW_H

#include "TileView.h"

#include "ResourceManager.h"

#include "../LibS/GeometryLight.h"

namespace sf
{
    class RenderTarget;
    class RenderStates;
    class Texture;
}
class MapLayer;
class Tile;

class PlainTileView : public TileView
{
public:
    PlainTileView(Tile* owner);
    PlainTileView(const PlainTileView& other);
    virtual ~PlainTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, const MapLayer& map);

    const ResourceHandle<sf::Texture> texture();
    const Geo::Vec2F spritePosition();

    virtual std::unique_ptr<TileView> clone() const;
protected:
    ResourceHandle<sf::Texture> m_texture;
    Geo::Vec2I m_spritePosition;
};

#endif // PLAINTILEVIEW_H
