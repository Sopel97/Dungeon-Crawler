#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "MapGenerator.h"

#include "Camera.h"

#include "EntitySystem.h"

#include "ResourceManager.h"

#include "window/WindowContent.h"

#include "../LibS/Geometry.h"

#include <SFML/Graphics.hpp>

class Root;
class MapLayer;
class TileLocation;
class Player;
class TallDrawable;
class IntenalWindow;
class TileMovedFromWorldToWorld;
class TileTransferMediator;

class World : public WindowContent
{
public:
    World(Root& root, Player& player, TileTransferMediator& tileTransferMediator, InternalWindow& wnd);
    ~World();

    void draw(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates);
    void drawOuterBorder(sf::RenderTarget & renderTarget, sf::RenderStates & renderStates, const TileLocation & tileLocation);

    void moveCamera(const ls::Vec2F& displacement);

    const Camera& camera() const;
    int width() const;
    int height() const;

    MapLayer& map();
    const MapLayer& map() const;
    const EntitySystem& entitySystem() const;
    const MapGenerator& mapGenerator() const;

    ls::Vec2I worldToTile(const ls::Vec2F& worldPosition) const;
    ls::Vec2F screenToWorld(const ls::Vec2I& screenPosition) const; //screenPosition is relative to top left of the window
    ls::Vec2I worldToScreen(const ls::Vec2F& worldPosition) const;
    ls::Vec2I screenToTile(const ls::Vec2I& screenPosition) const;
    ls::Vec2F tileCenterToWorld(const ls::Vec2I& tilePosition) const;

    void useTile(const ls::Vec2I& tilePosition);

    void update(float dt);

    float drag(const ls::Vec2F& position) const;

    bool isInsideWorldBounds(const ls::Vec2I& pos) const;

    std::vector<ls::Rectangle2F> queryTileColliders(const ls::Rectangle2F& queryRegion) const;
    std::vector<ls::Vec2I> queryGridPointsBetweenTiles(const ls::Vec2I& from, const ls::Vec2I& to) const;
    std::vector<ls::Vec2I> queryGridPointsBetweenPlayerAndTile(const ls::Vec2I& to) const;
    bool lineOfSightBetweenTiles(const ls::Vec2I& from, const ls::Vec2I& to) const;
    bool lineOfSightBetweenPlayerAndTile(const ls::Vec2I& to) const;
    float playerDistanceToTile(const ls::Vec2I& tile) const;
    int tileManhattanDistance(const ls::Vec2I& from, const ls::Vec2I& to) const;
    int tileManhattanDistanceFromPlayer(const ls::Vec2I& to) const;

    EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context);
    EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context);

protected:
    Root& m_root;
    Player& m_player;
    WindowSpaceManager& m_windowSpaceManager;
    TileTransferMediator& m_tileTransferMediator;
    int m_width;
    int m_height;
    std::unique_ptr<MapLayer> m_mapLayer;
    EntitySystem m_entitySystem;
    Camera m_camera;
    MapGenerator m_mapGenerator;

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

    std::vector<ls::Vec2I> queryGridPoints(const ls::LineSegment2F& line) const;
};

#endif // WORLD_H
