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
    virtual ~PlainTileView();

    virtual void loadFromConfiguration(ConfigurationNode& config);

    virtual void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, int x, int y, MapLayer& map);

    const ResourceHandle<sf::Texture> texture();
    const Geo::Vec2F spritePosition();
protected:
    ResourceHandle<sf::Texture> m_texture;
    Geo::Vec2I m_spritePosition;
};

#endif // PLAINTILEVIEW_H
