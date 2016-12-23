#include "EntitySystem.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "World.h"
#include "MapLayer.h"

#include "entities/Entity.h"
#include "entities/models/EntityModel.h"
#include "entities/views/EntityView.h"
#include "entities/controllers/EntityController.h"

#include "TileColumn.h"

#include "Camera.h"

#include <vector>
#include <algorithm>
#include <memory>

#include "GameConstants.h"

using namespace ls;

EntitySystem::EntitySystem()
{

}
EntitySystem::~EntitySystem()
{
    for(Entity* entity : m_entities)
    {
        delete entity;
    }
}

std::vector<Entity*> EntitySystem::queryRegion(const Rectangle2F& rect)
{
    std::vector<Entity*> entitiesInRegion;

    const Vec2F rectCenter = rect.centerOfMass();
    const float halfRectWidth = rect.width() / 2.0f;
    const float halfRectHeight = rect.height() / 2.0f;

    for(Entity* entity : m_entities)
    {
        const Vec2F entityPosition = entity->model().position();
        const float entityRadius = entity->model().colliderRadius();
        float xDist = std::abs(rectCenter.x - entityPosition.x) - halfRectWidth;
        float yDist = std::abs(rectCenter.y - entityPosition.y) - halfRectHeight;
        if(xDist < entityRadius && yDist < entityRadius)
        {
            entitiesInRegion.push_back(entity);
        }
    }

    return entitiesInRegion;
}
const std::vector<Entity*>& EntitySystem::entities() const
{
    return m_entities;
}

void EntitySystem::addEntity(Entity* newEntity, const ls::Vec2F& position)
{
    m_entities.push_back(newEntity);
    newEntity->model().setPosition(position);
}
void EntitySystem::removeEntity(Entity* entityToRemove)
{
    m_entities.erase(std::remove(m_entities.begin(), m_entities.end(), entityToRemove), m_entities.end());
}

void EntitySystem::updateEntities(World* world, float dt) //will also move them and resolve collisions
{
    for(Entity* entity : m_entities)
    {
        entity->controller().update(world, dt);
    }

    for(Entity* entity : m_entities)
    {
        moveEntity(world, entity, dt);
    }

    //TODO: make entities push each other
}
void EntitySystem::moveEntity(World* world, Entity* entity, float dt)
{
    auto& model = entity->model();
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
                const TileColumn& tileColumn = world->map().at(x, y);
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

    entity->controller().move(moveFactor, dt);
    model.setVelocity(velocity);
}

std::vector<Entity*> EntitySystem::getVisibleEntities(const Camera& camera)
{
    return queryRegion(camera.viewRectangle());
}
