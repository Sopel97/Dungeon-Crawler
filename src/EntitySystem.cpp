#include "EntitySystem.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "World.h"
#include "MapLayer.h"
#include "Player.h"

#include "entities/Entity.h"
#include "entities/EntityPrefab.h"
#include "entities/models/EntityModel.h"
#include "entities/renderers/EntityRenderer.h"
#include "entities/controllers/EntityController.h"

#include "colliders/Collisions.h"

#include "TileColumn.h"

#include "Camera.h"

#include <vector>
#include <algorithm>
#include <memory>

#include "GameConstants.h"

using namespace ls;

EntitySystem::EntitySystem(World& world, Player& player) :
    m_world(&world),
    m_player(&player)
{

}

std::vector<Entity*> EntitySystem::query(const Rectangle2F& rect)
{
    std::vector<Entity*> entitiesInRegion;

    const Vec2F rectCenter = rect.centerOfMass();
    const float halfRectWidth = rect.width() / 2.0f;
    const float halfRectHeight = rect.height() / 2.0f;

    for (std::unique_ptr<Entity>& entity : m_entities)
    {
        EntityCollider collider = entity->model().collider();
        const Vec2F pos = collider.volume().origin;
        const float radius = collider.volume().radius;
        const float xDist = std::abs(rectCenter.x - pos.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - pos.y) - halfRectHeight;
        if (xDist < radius && yDist < radius)
        {
            entitiesInRegion.push_back(entity.get());
        }
    }

    {
        Entity& playerEntity = m_player->entity();
        EntityCollider collider = playerEntity.model().collider();
        const Vec2F pos = collider.volume().origin;
        const float radius = collider.volume().radius;
        const float xDist = std::abs(rectCenter.x - pos.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - pos.y) - halfRectHeight;
        if (xDist < radius && yDist < radius)
        {
            entitiesInRegion.push_back(&playerEntity);
        }
    }

    return entitiesInRegion;
}
std::vector<EntityCollider> EntitySystem::queryColliders(const ls::Rectangle2F& rect)
{
    std::vector<EntityCollider> collidersInRegion;

    const Vec2F rectCenter = rect.centerOfMass();
    const float halfRectWidth = rect.width() / 2.0f;
    const float halfRectHeight = rect.height() / 2.0f;

    for (std::unique_ptr<Entity>& entity : m_entities)
    {
        EntityCollider collider = entity->model().collider();
        const Vec2F pos = collider.volume().origin;
        const float radius = collider.volume().radius;
        const float xDist = std::abs(rectCenter.x - pos.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - pos.y) - halfRectHeight;
        if (xDist < radius && yDist < radius)
        {
            collidersInRegion.push_back(collider);
        }
    }

    {
        Entity& playerEntity = m_player->entity();
        EntityCollider collider = playerEntity.model().collider();
        const Vec2F pos = collider.volume().origin;
        const float radius = collider.volume().radius;
        const float xDist = std::abs(rectCenter.x - pos.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - pos.y) - halfRectHeight;
        if (xDist < radius && yDist < radius)
        {
            collidersInRegion.push_back(collider);
        }
    }

    return collidersInRegion;
}
const std::vector<std::unique_ptr<Entity>>& EntitySystem::entities() const
{
    return m_entities;
}
Entity& EntitySystem::entity(int i)
{
    return *(m_entities[i]);
}
const Entity& EntitySystem::entity(int i) const
{
    return *(m_entities[i]);
}

Entity& EntitySystem::addEntity(std::unique_ptr<Entity>&& newEntity)
{
    m_entities.emplace_back(std::move(newEntity));
    return *(m_entities.back());
}
Entity& EntitySystem::spawnEntity(const EntityPrefab& prefab, const ls::Vec2F& position)
{
    return addEntity(prefab.instantiate(position));
}
void EntitySystem::removeEntity(Entity& entityToRemove)
{
    m_entities.erase(std::find_if(m_entities.begin(), m_entities.end(), [&entityToRemove](std::unique_ptr<Entity>& ent) {return ent.get() == &entityToRemove; }), m_entities.end());
}
void EntitySystem::removeDeadEntities()
{
    m_entities.erase(std::remove_if(m_entities.begin(), m_entities.end(), [this](std::unique_ptr<Entity>& ent) {return isDead(*ent); }), m_entities.end());
}
bool EntitySystem::isDead(const Entity& entity) const
{
    return entity.model().health() <= 0;
}
void EntitySystem::createCorpsesForDeadEntities(World& world) const
{
    MapLayer& map = world.map();

    for (const std::unique_ptr<Entity>& ent : m_entities)
    {
        const Entity& entity = *ent;
        if (isDead(entity))
        {
            const ls::Vec2F pos = entity.model().position();
            const ls::Vec2I tilePos = world.worldToTile(pos);
            TileStack corpse = createCorpse(entity);
            if (corpse.isEmpty()) continue;

            map.placeTile(std::move(corpse), tilePos.x, tilePos.y);
        }
    }
}
TileStack EntitySystem::createCorpse(const Entity& entity) const
{
    return entity.model().createCorpse();
}

void EntitySystem::update(float dt) //will also move them and resolve collisions
{
    for (std::unique_ptr<Entity>& entity : m_entities)
    {
        updateEntity(*entity, dt);
    }
    updateEntity(m_player->entity(), dt);

    //TODO: make entities push each other

    createCorpsesForDeadEntities(*m_world);
    removeDeadEntities();
}
void EntitySystem::updateEntity(Entity& entity, float dt)
{
    auto& model = entity.model();

    const Vec2F positionBeforeUpdate = model.position();
    entity.controller().update(*m_world, dt);
    const Vec2F positionAfterUpdate = model.position();
    Vec2F currentPosition = positionAfterUpdate;
    Vec2F currentVelocity = model.velocity();

    EntityCollider entityCollider = model.collider();

    std::vector<TileCollider> tileCollidersInRange = m_world->queryTileColliders(entityCollider.boundingBox());

    [&]()
    {
        constexpr int maxIterations = 2;
        for (int i = 0; i < maxIterations; ++i)
        {
            auto pen = penetration(entityCollider, tileCollidersInRange);
            if (pen.has_value())
            {
                const ls::Vec2F r = resolvePenetration(pen.value());
                currentPosition += r;
                model.setPosition(currentPosition);
                entityCollider = model.collider();
            }
            else return;
        }

        { // max 2 iterations, on third just restore old position
            auto pen = penetration(entityCollider, tileCollidersInRange);
            if (pen.has_value())
            {
                model.setPosition(positionBeforeUpdate);
                model.setVelocity(ls::Vec2F(0.0f, 0.0f));
            }
        }
    }();
    model.setVelocity(adjustedVelocityAfterPenetrationResolved(currentPosition - positionAfterUpdate, currentVelocity));
}
bool EntitySystem::isAlmostZero(const ls::Vec2F& v, float thr) const
{
    return (std::abs(v.x) < thr && std::abs(v.y) < thr);
}
ls::Vec2F EntitySystem::adjustedVelocityAfterPenetrationResolved(const ls::Vec2F& escape, const ls::Vec2F& velocity)
{
    if (isAlmostZero(escape, 0.000001f)) return velocity;

    const ls::Vec2F escapeNormal = escape.normalized().normal();
    const ls::Vec2F fac(std::abs(escapeNormal.x), std::abs(escapeNormal.y));

    return velocity * fac;
}
ls::Vec2F EntitySystem::resolvePenetration(const ls::Vec2F& pen)
{
    if (std::abs(pen.x) < std::abs(pen.y))
    {
        return { -pen.x*1.01f, 0.0f };
    }
    else
    {
        return { 0.0f, -pen.y*1.01f };
    }
}
std::optional<ls::Vec2F> EntitySystem::penetration(EntityCollider& entityCollider, std::vector<TileCollider>& tileColliders)
{
    for (auto& tileCollider : tileColliders)
    {
        if (Collisions::collide(tileCollider, entityCollider))
        {
            return penetration(entityCollider, tileCollider);
        }
    }

    return std::nullopt;
}
ls::Vec2F EntitySystem::penetration(EntityCollider& entityCollider, TileCollider& tileCollider)
{
    const Rectangle2F& tileAABB = tileCollider.boundingBox();
    const Rectangle2F& entityAABB = entityCollider.boundingBox();

    float xPen;
    if (tileAABB.min.x < entityAABB.min.x)
    {
        xPen = entityAABB.min.x - tileAABB.max.x;
    }
    else
    {
        xPen = entityAABB.max.x - tileAABB.min.x;
    }

    float yPen;
    if (tileAABB.min.y < entityAABB.min.y)
    {
        yPen = entityAABB.min.y - tileAABB.max.y;
    }
    else
    {
        yPen = entityAABB.max.y - tileAABB.min.y;
    }

    return { xPen, yPen };
}

std::vector<Entity*> EntitySystem::getVisibleEntities(const Camera& camera)
{
    return query(camera.viewRectangle());
}
