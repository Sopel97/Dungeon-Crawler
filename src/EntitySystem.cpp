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

#include "TileColumn.h"

#include "Camera.h"

#include <vector>
#include <algorithm>
#include <memory>

#include "GameConstants.h"

using namespace ls;

EntitySystem::EntitySystem(Player& player) :
    m_player(&player)
{

}

std::vector<Entity*> EntitySystem::query(const Rectangle2F& rect)
{
    std::vector<Entity*> entitiesInRegion;

    const Vec2F rectCenter = rect.centerOfMass();
    const float halfRectWidth = rect.width() / 2.0f;
    const float halfRectHeight = rect.height() / 2.0f;

    for(std::unique_ptr<Entity>& entity : m_entities)
    {
        const Vec2F entityPosition = entity->model().position();
        const float entityRadius = entity->model().colliderRadius();
        const float xDist = std::abs(rectCenter.x - entityPosition.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - entityPosition.y) - halfRectHeight;
        if(xDist < entityRadius && yDist < entityRadius)
        {
            entitiesInRegion.push_back(entity.get());
        }
    }

    {
        Entity& playerEntity = m_player->entity();
        const Vec2F position = playerEntity.model().position();
        const float radius = playerEntity.model().colliderRadius();
        const float xDist = std::abs(rectCenter.x - position.x) - halfRectWidth;
        const float yDist = std::abs(rectCenter.y - position.y) - halfRectHeight;
        if (xDist < radius && yDist < radius)
        {
            entitiesInRegion.push_back(&playerEntity);
        }
    }

    return entitiesInRegion;
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

void EntitySystem::addEntity(std::unique_ptr<Entity>&& newEntity, const ls::Vec2F& position)
{
    m_entities.emplace_back(std::move(newEntity));
    newEntity->model().setPosition(position);
}
void EntitySystem::emplaceEntity(const EntityPrefab& prefab, const ls::Vec2F& position)
{
    std::unique_ptr<Entity> newEntity = prefab.instantiate();
    newEntity->model().setPosition(position);
    m_entities.emplace_back(std::move(newEntity));
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

void EntitySystem::updateEntities(World& world, float dt) //will also move them and resolve collisions
{
    for(std::unique_ptr<Entity>& entity : m_entities)
    {
        entity->controller().update(world, dt);
    }
    m_player->entity().controller().update(world, dt);

    for(std::unique_ptr<Entity>& entity : m_entities)
    {
        moveEntity(world, *entity, dt);
    }
    moveEntity(world, m_player->entity(), dt);

    //TODO: make entities push each other

    createCorpsesForDeadEntities(world);
    removeDeadEntities();
}
void EntitySystem::moveEntity(World& world, Entity& entity, float dt)
{
    auto& model = entity.model();
    Vec2F displacementWhenMoved = model.displacementWhenMoved(dt);
    Vec2F position = model.position();
    Vec2F velocity = model.velocity();
    Vec2F idealPositionAfterMove = position + displacementWhenMoved;
    Vec2F moveFactor(1.0f, 1.0f);

    if(model.hasCollider())
    {
        float entityColliderRadius = model.colliderRadius();
        Rectangle2F entityCollider(idealPositionAfterMove - Vec2F(entityColliderRadius, entityColliderRadius), idealPositionAfterMove + Vec2F(entityColliderRadius, entityColliderRadius));
        int xmin = static_cast<int>(entityCollider.min.x / static_cast<float>(GameConstants::tileSize));
        int ymin = static_cast<int>(entityCollider.min.y / static_cast<float>(GameConstants::tileSize));
        int xmax = static_cast<int>(entityCollider.max.x / static_cast<float>(GameConstants::tileSize));
        int ymax = static_cast<int>(entityCollider.max.y / static_cast<float>(GameConstants::tileSize));

        std::vector<Rectangle2F> collidersInRange;

        for(int x = xmin; x <= xmax; ++x)
        {
            for(int y = ymin; y <= ymax; ++y)
            {
                const TileColumn& tileColumn = world.map().at(x, y);
                if(tileColumn.hasCollider())
                {
                    Rectangle2F tileCollider = tileColumn.collider().translated(Vec2F(static_cast<float>(x), static_cast<float>(y)) * static_cast<float>(GameConstants::tileSize));
                    collidersInRange.push_back(tileCollider);
                }
            }
        }

        entityCollider.translate(-displacementWhenMoved); //back to initial position
        entityCollider.translate(Vec2F(displacementWhenMoved.x, 0.0f)); //as it moved only in x direction

        for(const auto& rect : collidersInRange)
        {
            if(ls::intersect(entityCollider, rect))
            {
                moveFactor.x = 0.0f;
                entityCollider.translate(-Vec2F(displacementWhenMoved.x, 0.0f)); //if it can't move there go back
                velocity.x = 0.0f;
                break;
            }
        }

        entityCollider.translate(Vec2F(0.0f, displacementWhenMoved.y)); //as it moved in y direction

        for(const auto& rect : collidersInRange)
        {
            if(ls::intersect(entityCollider, rect))
            {
                moveFactor.y = 0.0f;
                velocity.y = 0.0f;
                break;
            }
        }
    }

    entity.controller().move(moveFactor, dt);
    model.setVelocity(velocity);
}

std::vector<Entity*> EntitySystem::getVisibleEntities(const Camera& camera)
{
    return query(camera.viewRectangle());
}
