#include "EntitySystem.h"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

#include "Entity.h"
#include "EntityModel.h"
#include "EntityView.h"
#include "EntityController.h"

#include "Camera.h"

#include <vector>
#include <algorithm>
#include <memory>
#include "../LibS/make_unique.h"

using namespace Geo;

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

std::vector<Entity*> EntitySystem::queryRegion(const RectangleF& rect)
{
    std::vector<Entity*> entitiesInRegion;

    const Vec2F rectCenter = rect.center();
    const float halfRectWidth = rect.width()/2.0f;
    const float halfRectHeight = rect.height()/2.0f;

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

void EntitySystem::addEntity(Entity* newEntity)
{
    m_entities.push_back(newEntity);
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
}
void EntitySystem::drawVisibleEntities(sf::RenderTarget& renderTarget, sf::RenderStates& renderStates, const Camera& camera)
{
    for(Entity* entity : queryRegion(camera.viewRectangle()))
    {
        entity->view().draw(renderTarget, renderStates);
    }
}
