#ifndef WORLD_H
#define WORLD_H

#include <memory>

#include "MapGenerator.h"

#include "Camera.h"

#include "EntitySystem.h"
#include "ProjectileSystem.h"

#include "ResourceManager.h"

#include "WorldRenderer.h"

#include "window/WindowContent.h"

#include "colliders/TileCollider.h"

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
    //TEMP?
    friend class WorldRenderer;
public:
    World(Root& root, Player& player, TileTransferMediator& tileTransferMediator);
    ~World();

    void draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates) override;

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
    void lookTile(const ls::Vec2I& tilePosition);

    void update(float dt);

    float drag(const ls::Vec2F& position) const;

    bool isInsideWorldBounds(const ls::Vec2I& pos) const;

    std::vector<ls::Rectangle2F> queryLightOccluders(const Light& light) const;
    std::vector<TileCollider> queryTileColliders(const ls::Rectangle2F& queryRegion);
    std::vector<Light> queryLights(const ls::Rectangle2F& queryRegion) const;
    std::vector<ls::Vec2I> queryGridPointsBetweenTiles(const ls::Vec2I& from, const ls::Vec2I& to) const;
    std::vector<ls::Vec2I> queryGridPointsBetweenEntityAndTile(const Entity& entity, const ls::Vec2I& to) const;
    bool lineOfSightBetweenTiles(const ls::Vec2I& from, const ls::Vec2I& to) const;
    bool lineOfSightBetweenEntityAndTile(const Entity& entity, const ls::Vec2I& to) const;
    float playerDistanceToTile(const ls::Vec2I& tile) const;
    int tileManhattanDistance(const ls::Vec2I& from, const ls::Vec2I& to) const;
    int tileManhattanDistanceFromPlayer(const ls::Vec2I& to) const;

    Projectile& spawnProjectile(const ProjectilePrefab& prefab, World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition);
    Entity& spawnEntity(const EntityPrefab& prefab, const ls::Vec2F& position);
    bool trySpawnTile(const TilePrefab& prefab, int x, int y, int quantity);
    bool trySpawnTileNearby(const TilePrefab& prefab, int x, int y, int quantity);
    bool tryPlaceTile(TileStack&& tileStack, int x, int y);
    bool tryPlaceTileNearby(TileStack&& tileStack, int x, int y);

    EventResult onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context) override;
    EventResult onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context) override;
    EventResult onKeyPressed(sf::Event::KeyEvent& event, EventContext context) override;

    void onEntityDeleted(Entity& entity);

    bool isWithinWorldWindow(const ls::Vec2I& pos) const;

protected:
    WorldRenderer m_worldRenderer;
    Root& m_root;
    Player& m_player;
    WindowSpaceManager& m_windowSpaceManager;
    TileTransferMediator& m_tileTransferMediator;
    int m_width;
    int m_height;
    std::unique_ptr<MapLayer> m_mapLayer;
    EntitySystem m_entitySystem;
    ProjectileSystem m_projectileSystem;
    MapGenerator m_mapGenerator;

    static constexpr int m_worldWidth = 128;
    static constexpr int m_worldHeight = 128;

protected:
    std::vector<ls::Vec2I> queryGridPoints(const ls::LineSegment2F& line) const;
};

#endif // WORLD_H
