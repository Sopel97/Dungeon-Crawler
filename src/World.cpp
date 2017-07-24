#include "World.h"

#include "Root.h"
#include "Game.h"

#include "tiles/Tile.h"
#include "tiles/TileStack.h"
#include "tiles/renderers/TileRenderer.h"
#include "tiles/models/TileModel.h"

#include "TileLocation.h"

#include "entities/Entity.h"
#include "entities/EntityPrefab.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"

#include "TileColumn.h"
#include "MapLayer.h"

#include "GameTime.h"

#include "window/WindowSpaceManager.h"

#include "Light.h"

#include "TileTransferMediator.h"

#include <SFML/System.hpp>

#include "../LibS/Util.h"

#include "GameConstants.h"

#include "Logger.h"

using namespace ls;

World::World(Root& root, Player& player, TileTransferMediator& tileTransferMediator) :
    m_worldRenderer(root, *this),
    m_root(root),
    m_player(player),
    m_windowSpaceManager(root.windowSpaceManager()),
    m_tileTransferMediator(tileTransferMediator),
    m_width(m_worldWidth),
    m_height(m_worldHeight),
    m_mapLayer(std::make_unique<MapLayer>(*this, m_width, m_height)),
    m_entitySystem(*this, player),
    m_projectileSystem(*this, m_entitySystem),
    m_mapGenerator(m_width, m_height)
{
    m_mapGenerator.generate(*m_mapLayer);
    m_player.entity().model().setPosition(Vec2F(m_width * GameConstants::tileSize / 2.0f, m_height * GameConstants::tileSize / 2.0f));
}
World::~World()
{

}

void World::draw(sf::RenderTarget& renderTarget, const sf::RenderStates& renderStates)
{
    m_worldRenderer.draw(renderTarget, renderStates);
}

int World::width() const
{
    return m_width;
}
int World::height() const
{
    return m_height;
}

MapLayer& World::map()
{
    return *m_mapLayer;
}
const MapLayer& World::map() const
{
    return *m_mapLayer;
}

const EntitySystem& World::entitySystem() const
{
    return m_entitySystem;
}
const MapGenerator& World::mapGenerator() const
{
    return m_mapGenerator;
}

Vec2I World::worldToTile(const Vec2F& worldPosition) const
{
    return Vec2I(Util::fastFloor(worldPosition.x / static_cast<float>(GameConstants::tileSize)), Util::fastFloor(worldPosition.y / static_cast<float>(GameConstants::tileSize)));
}
ls::Vec2F World::screenToWorld(const ls::Vec2I& screenPosition) const
{
    const sf::Vector2f worldPosition = m_root.window().mapPixelToCoords(sf::Vector2i(screenPosition.x, screenPosition.y), m_windowSpaceManager.getContentView(window(), m_worldRenderer.camera().viewRectangle()));
    return{ worldPosition.x, worldPosition.y };
}
ls::Vec2I World::worldToScreen(const ls::Vec2F& worldPosition) const
{
    const sf::Vector2i screenPosition = m_root.window().mapCoordsToPixel(sf::Vector2f(worldPosition.x, worldPosition.y), m_windowSpaceManager.getContentView(window(), m_worldRenderer.camera().viewRectangle()));
    return{ screenPosition.x, screenPosition.y };
}
ls::Vec2I World::screenToTile(const ls::Vec2I& screenPosition) const
{
    return worldToTile(screenToWorld(screenPosition));
}
ls::Vec2F World::tileCenterToWorld(const ls::Vec2I& tilePosition) const
{
    const ls::Vec2F tileSize{ static_cast<float>(GameConstants::tileSize), static_cast<float>(GameConstants::tileSize) };
    const ls::Vec2F tilePos = static_cast<ls::Vec2F>(tilePosition) * tileSize + tileSize / 2.0f;
    return tilePos;
}

void World::useTile(const ls::Vec2I& tilePosition)
{
    TileColumn& tileColumn = m_mapLayer->at(tilePosition.x, tilePosition.y);
    tileColumn.top().tile().model().use(m_root.game().player(), TileLocation(*m_mapLayer, tilePosition.x, tilePosition.y, tileColumn.topZ()));
}
void World::lookTile(const ls::Vec2I& tilePosition)
{
    TileColumn& tileColumn = m_mapLayer->at(tilePosition.x, tilePosition.y);
    tileColumn.top().tile().model().look(m_root.game().player(), TileLocation(*m_mapLayer, tilePosition.x, tilePosition.y, tileColumn.topZ()));
}

void World::update(float dt)
{
    m_entitySystem.update(dt);
    m_projectileSystem.update(dt);

    m_worldRenderer.setCamera(m_player.entity().model().position());
}

float World::drag(const Vec2F& position) const
{
    Vec2I tilePosition = worldToTile(position);
    const TileColumn& tileColumn = m_mapLayer->at(tilePosition.x, tilePosition.y);
    const TileStack& tileStack = tileColumn.at(0);
    return tileStack.tile().model().drag();
}

bool World::isInsideWorldBounds(const ls::Vec2I& pos) const
{
    return pos.x >= 0 && pos.y >= 0 && pos.x < m_width && pos.y < m_height;
}

std::vector<TileCollider> World::queryTileColliders(const Rectangle2F& queryRegion)
{
    return m_mapLayer->queryTileColliders(queryRegion);
}
std::vector<ls::Vec2I> World::queryGridPointsBetweenTiles(const ls::Vec2I& from, const ls::Vec2I& to) const
{
    return queryGridPoints(ls::LineSegment2F(ls::Vec2F(from.x + 0.5f, from.y + 0.5f), ls::Vec2F(to.x + 0.5f, to.y + 0.5f)));
}
std::vector<ls::Vec2I> World::queryGridPointsBetweenPlayerAndTile(const ls::Vec2I& to) const
{
    return queryGridPoints(ls::LineSegment2F(m_player.entity().model().position() / static_cast<float>(GameConstants::tileSize), ls::Vec2F(to.x + 0.5f, to.y + 0.5f)));
}
bool World::lineOfSightBetweenTiles(const ls::Vec2I& from, const ls::Vec2I& to) const
{
    for (auto pos : queryGridPointsBetweenTiles(from, to))
    {
        if (!m_mapLayer->at(pos.x, pos.y).top().tile().model().isThrowableThrough()) return false;
    }
    return true;
}
bool World::lineOfSightBetweenPlayerAndTile(const ls::Vec2I& to) const
{
    for (auto pos : queryGridPointsBetweenPlayerAndTile(to))
    {
        if (!m_mapLayer->at(pos.x, pos.y).top().tile().model().isThrowableThrough()) return false;
    }
    return true;
}
float World::playerDistanceToTile(const ls::Vec2I& tile) const
{
    const ls::Vec2F playerPos = m_player.entity().model().position();
    const ls::Vec2F tilePos = tileCenterToWorld(tile);
    return playerPos.distance(tilePos);
}
int World::tileManhattanDistance(const ls::Vec2I& from, const ls::Vec2I& to) const
{
    return std::abs(from.x - to.x) + std::abs(from.y - to.y);
}
int World::tileManhattanDistanceFromPlayer(const ls::Vec2I& to) const
{
    return tileManhattanDistance(worldToTile(m_player.entity().model().position()), to);
}
Projectile& World::spawnProjectile(const ProjectilePrefab& prefab, World& world, Entity& parentEntity, const ls::Vec2F& hintedPosition)
{
    return m_projectileSystem.spawnProjectile(prefab, world, parentEntity, hintedPosition);
}
Entity& World::spawnEntity(const EntityPrefab& prefab, const ls::Vec2F& position)
{
    return m_entitySystem.spawnEntity(prefab, position);
}
std::vector<ls::Vec2I> World::queryGridPoints(const ls::LineSegment2F& line) const
{
    constexpr float eps = 0.01f; //tolerance for diagonal moves

    // we have to convert the line so (end - begin) has positive x and y
    ls::Vec2F begin = line.begin;
    ls::Vec2F end = line.end;
    if (std::abs(end.x - begin.x) < eps && std::abs(end.y - begin.y) < eps) return { { ls::Util::fastFloor(begin.x), ls::Util::fastFloor(begin.y) } };

    if (begin.x > end.x) std::swap(begin, end);

    // begin.x < end.x always
    const int mirror = end.y > begin.y ? 1 : -1;
    const int displacement = end.y > begin.y ? 0 : -1;
    begin.y *= mirror;
    end.y *= mirror;
    const ls::Vec2F direction = (end - begin).normalized();

    ls::Vec2F currentPos = begin;
    ls::Vec2I currentGridPos(ls::Util::fastFloor(begin.x), ls::Util::fastFloor(begin.y));
    ls::Vec2F nextGridLines(currentGridPos.x + 1.0f, currentGridPos.y + 1.0f);

    std::vector<ls::Vec2I> points;
    points.reserve(ls::Util::fastFloor(end.x - begin.x) + ls::Util::fastFloor(end.y - begin.y) + 3); // little overestimation
    for (;;)
    {
        points.emplace_back(currentGridPos.x, currentGridPos.y * mirror + displacement);

        const float distX = nextGridLines.x - currentPos.x;
        const float distY = nextGridLines.y - currentPos.y;

        const float dtx = distX / direction.x;
        const float dty = distY / direction.y;

        float dt;
        if (std::abs(dtx - dty) < eps) //move diagonal
        {
            ++currentGridPos.x;
            ++currentGridPos.y;
            nextGridLines.x += 1.0f;
            nextGridLines.y += 1.0f;
            dt = std::max(dtx, dty);
        }
        else if (dtx < dty) // move right
        {
            ++currentGridPos.x;
            nextGridLines.x += 1.0f;
            dt = dtx;
        }
        else // move up
        {
            ++currentGridPos.y;
            nextGridLines.y += 1.0f;
            dt = dty;
        }

        currentPos.x += dt * direction.x;
        currentPos.y += dt * direction.y;

        if (end.x - currentPos.x < eps && end.y - currentPos.y < eps) break;
    }

    return points;
}

auto World::onMouseButtonPressed(sf::Event::MouseButtonEvent& event, EventContext context)
-> EventResult
{
    if (!context.isMouseOver) return EventResult{}.setTakeFocus(false).setConsumeEvent(false);

    if (event.button == sf::Mouse::Button::Right)
    {
        const Rectangle2I& worldViewRect = window().absoluteContentRect();
        if (ls::intersect(worldViewRect, Vec2I(event.x, event.y))) //TODO: may be redundant
        {
            const Vec2I tilePosition = screenToTile(Vec2I(event.x, event.y));

            if (isInsideWorldBounds(tilePosition))
            {
                useTile(tilePosition);
            }
        }
    }
    else if (event.button == sf::Mouse::Button::Left)
    {
        const Rectangle2I& worldViewRect = window().absoluteContentRect();
        if (ls::intersect(worldViewRect, Vec2I(event.x, event.y))) //TODO: may be redundant
        {
            const Vec2I tilePosition = screenToTile(Vec2I(event.x, event.y));

            if (isInsideWorldBounds(tilePosition))
            {
                if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::LAlt))
                {
                    lookTile(tilePosition);
                }
                else
                {
                    m_tileTransferMediator.grabFromWorld(tilePosition, *this, m_player);
                }
            }
        }
    }

    return EventResult{}.setTakeFocus().setConsumeEvent();
}

auto World::onMouseButtonReleased(sf::Event::MouseButtonEvent& event, EventContext context)
-> EventResult
{
    if (!context.isMouseOver)
    {
        if (event.button == sf::Mouse::Button::Left)
        {
            m_tileTransferMediator.reset();
            Logger::instance().log(Logger::Priority::Debug, "reset tile move from world");
        }

        return EventResult{}.setTakeFocus(false).setConsumeEvent(false);
    }

    if (event.button == sf::Mouse::Button::Left)
    {
        const Rectangle2I& worldViewRect = window().absoluteContentRect();
        if (ls::intersect(worldViewRect, Vec2I(event.x, event.y))) //TODO: may be redundant
        {
            const Vec2I tilePosition = screenToTile(Vec2I(event.x, event.y));

            if (isInsideWorldBounds(tilePosition))
            {
                m_tileTransferMediator.putToWorld(tilePosition, *this, m_player);
            }
            else
            {
                m_tileTransferMediator.reset();
            }
        }
    }

    return EventResult{}.setTakeFocus().setConsumeEvent();
}
auto World::onKeyPressed(sf::Event::KeyEvent& event, EventContext context) 
-> EventResult
{
    if (event.code == sf::Keyboard::Num1)
    {
        const ls::Vec2I mousePos(sf::Mouse::getPosition(m_windowSpaceManager.window()).x, sf::Mouse::getPosition(m_windowSpaceManager.window()).y);
        const ls::Vec2F worldPos = screenToWorld(mousePos);
        m_entitySystem.spawnEntity(ResourceManager::instance().get<EntityPrefab>("test").get(), worldPos);
    
        return EventResult().setConsumeEvent().setTakeFocus(false);
    }
    else if (event.code == sf::Keyboard::Num2)
    {
        const ls::Vec2I mousePos(sf::Mouse::getPosition(m_windowSpaceManager.window()).x, sf::Mouse::getPosition(m_windowSpaceManager.window()).y);
        const ls::Vec2F worldPos = screenToWorld(mousePos);
        m_player.attack(*this, worldPos);

        return EventResult().setConsumeEvent().setTakeFocus(false);
    }
    return EventResult().setConsumeEvent(false).setTakeFocus(false);
}
void World::onEntityDeleted(Entity& entity)
{
    m_projectileSystem.onEntityDeleted(entity);
}