#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "MapGenerator.h"

#include "Camera.h"

#include "EntitySystem.h"

#include "ResourceManager.h"

#include "WindowSpaceUser.h"

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>

class Root;
class MapLayer;
class TileLocation;
class Player;
class TallDrawable;

class World : public WindowSpaceUser
{
public:
    World(Root& root, Player& player);
    ~World();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawOuterBorder(sf::RenderTarget & renderTarget, sf::RenderStates & renderStates, const TileLocation & tileLocation);

    void moveCamera(const ls::Vec2F& displacement);

    const Camera& camera() const;
    int width() const;
    int height() const;

    const MapLayer& map() const;
    const EntitySystem& entitySystem() const;
    const MapGenerator& mapGenerator() const;

    ls::Vec2I worldToTile(const ls::Vec2F& worldPosition) const;
    ls::Vec2F screenToWorld(const ls::Vec2I& screenPosition) const; //screenPosition is relative to top left of the window
    ls::Vec2I worldToScreen(const ls::Vec2F& worldPosition) const;
    ls::Vec2I screenToTile(const ls::Vec2I& screenPosition) const;

    void useTile(const ls::Vec2I& tilePosition);

    void update(float dt);

    float drag(const ls::Vec2F& position) const;

    std::vector<ls::Rectangle2F> queryTileColliders(const ls::Rectangle2F& queryRegion) const;
protected:
    Root& m_root;
    int m_width;
    int m_height;
    std::unique_ptr<MapLayer> m_mapLayer;
    EntitySystem m_entitySystem;
    Camera m_camera;
    MapGenerator m_mapGenerator;
    Entity* m_playerEntity;

    sf::RenderTexture m_intermidiateRenderTarget;
    sf::RenderTexture m_lightMap;
    sf::RenderTexture m_metaTexture;
    sf::Shader m_prettyStretchShader;
    sf::Shader m_lightShader;

    ResourceHandle<sf::Texture> m_lightTexture;

    static constexpr int m_viewWidth = 15;
    static constexpr int m_viewHeight = 15;
    static constexpr int m_worldWidth = 128;
    static constexpr int m_worldHeight = 128;

protected:
    void prepareIntermidiateRenderTarget();
    void prepareLightMap();
    void prepareMetaTexture();
    void updateShaderUniforms();
    void drawMeta(sf::RenderStates& renderStates, const std::vector<TallDrawable*>& tallDrawables);
    void drawIntermidiate(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawLightMapToIntermidiate(sf::RenderStates& renderStates);
    void drawLightsToLightMap();
};

#endif // WORLD_H
